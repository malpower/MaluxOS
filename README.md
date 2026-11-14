# MaluxOS

MaluxOS is a small 32-bit x86 hobby operating system written in C and NASM. It boots from a legacy BIOS, switches to protected mode, enables paging, runs a preemptive multitasking kernel, and provides a simple graphical shell with its own window manager and filesystem.

The project was originally uploaded in 2014 and is primarily a learning/experimental OS. It targets legacy BIOS hardware and Bochs/QEMU-style VBE graphics. The in-tree build scripts still assume the original 2014-era i386 toolchain (see [Building](#building)).

- **Boot image:** `malux005.img` (1.44 MB raw disk image, version 0.05)
- **Architecture:** x86, 32-bit protected mode, ring 0/ring 3
- **Graphics:** VBE mode `0x118` (1024 × 768 × 24 bpp)
- **Filesystem:** malFS (custom, sector-based)
- **Author:** michael lou `<malpower@ymail.com>`
- **Repository:** <https://github.com/malpower/MaluxOS>

---

## Highlights

- BIOS bootloader (MBR) that loads the kernel and enters protected mode
- Flat 32-bit kernel with GDT, IDT, TSS and per-process LDTs
- Paging with a ~4 GB identity map and a high-half kernel alias
- Preemptive multitasking driven by the PIT (IRQ0)
- User processes in ring 3, communicating through software-interrupt syscalls
- Custom window manager, text rendering and 24-bpp framebuffer output
- ATA PIO block driver, PS/2 keyboard driver, VBE graphics
- malFS filesystem with directories, files, and a sector allocation table
- A small shell (`BUI.SYS`) and a sample user program (`mal.pgm`)

---

## Quick start (prebuilt image)

The repository ships a ready-to-boot `malux005.img`. On Linux:

```sh
# Debian/Ubuntu (x86-64 or ARM64 host)
sudo apt install qemu-system-x86

qemu-system-i386 -machine pc -m 128 \
  -drive file=malux005.img,format=raw,if=ide,index=0,media=disk \
  -vga std -no-reboot
```

Notes:

- The bootloader expects the image as the **first IDE hard disk** (`DL = 0x80`). Do not use `-fda`; the 1.44 MB size is incidental.
- Use the default `pc` machine (i440fx/PIIX3) so the legacy ATA ports at `0x1F0` are available.
- Use `-vga std` (or `cirrus`) so VBE mode `0x118` is available.
- Use SeaBIOS (the default). The OS relies on `int 0x10`, `int 0x13` and `int 0x15` real-mode services, so OVMF/UEFI will not work.
- On ARM64 hosts QEMU uses TCG; do **not** pass `-enable-kvm` for an x86 guest.
- On a headless machine, add `-display none -vnc :0` and connect a VNC viewer. There is no serial console.
- `-no-reboot` stops on a triple fault instead of rebooting. Add
  `-d int,cpu_reset -D qemu.log` to log the last exception.

---

## Boot flow

1. The BIOS loads the 512-byte MBR (`Booter/booterHDD.asm`) at `0x7C00`.
2. The MBR asks VBE for mode `0x118` (1024×768×24) using `int 0x10`,
   `AX = 0x4F01` / `0x4F02`.
3. It reads **64 sectors** from CHS `(cylinder 0, head 0, sector 2)` /
   LBA 1 into `0x1000:0` = `0x10000` using `int 0x13, AH = 0x02`.
4. It records the extended-memory size from `int 0x15, AH = 0x88` at
   `0x1000:0x7FF0`.
5. It enables A20 through port `0x92`, enables protected mode, and jumps to
   the temporary 32-bit trampoline.
6. The trampoline copies `0xFFFF` bytes from `0x10000` to `0x100000` (1 MB),
   loads the data/stack selectors, and jumps to `CS = 0x10, EIP = 0`.
7. The kernel entry point is `main()` in `Kernel/main.c`:

   ```c
   InitMem();      // paging + new GDT
   InitInt();      // IDT
   InitTSS();      // TSS / TR
   Installer();    // IDT gates, PIC, syscalls, drivers, memory manager, FS
   GetInUI();      // drop to ring 3 and start the shell
   ```

8. `InitPage` builds a page directory at physical `0x200000` and about 4 MB of
   page tables at `0x201000`, identity-mapping roughly 4 GB and enabling paging.
   `PDE[1023]` deliberately points at the same page table as `PDE[0]`, so the
   top 4 MB of the linear address space aliases the first 4 MB. This is how the
   high-half kernel mapping works (see [Memory layout](#memory-layout)).
9. `InitSeg` writes a new GDT at `0x30000` using high-half segment bases such as
   `0xFFD00000` for kernel code/data, then reloads the GDT and the data/stack
   selectors.
10. `Installer` installs the interrupt gates, remaps the PIC, registers the
    syscalls and drivers, and initializes the memory manager and filesystem.
11. `GetInUI` creates the first user process and returns to ring 3 through an
    LDT, starting the shell (`BUI.SYS`).

---

## Memory layout

The kernel uses a fixed physical layout. These addresses are hard-coded in the
kernel and bootloader, not discovered from firmware tables.

| Address | Size | Purpose |
|---|---|---|
| `0x00000000` | — | Real-mode IVT/BDA, low memory |
| `0x00030000` | ~328 B | New GDT (41 descriptors) |
| `0x00090000` | — | VBE mode-info block filled by the BIOS |
| `0x00100000` | ~24 KB | Kernel image (also aliased at `0xFFD00000`) |
| `0x00108000` | 8 KB | PCB list (`ADDRESS_PCB_LIST = 0x8000` in the kernel segment) |
| `0x0010FFF0` | — | Kernel stack top (grows down) |
| `0x00140000` | 2 KB | IDT (256 gates) |
| `0x00150000` | 4 KB | TSS |
| `0x00151000` | ~8 KB | LDT area (up to 31 × 256 B) |
| `0x00200000` | 4 KB | Page directory (`CR3`) |
| `0x00201000` | ~4 MB | Page tables |
| `0x00D60000` | 1 MB | Physical page bitmap used by the memory manager |
| VBE LFB | — | Linear framebuffer address reported by VBE (typically `0xFD000000` on QEMU std VGA) |

Because of the `PDE[1023]` alias:

- `0xFFC00000`–`0xFFFFFFFF` maps to physical `0x00000000`–`0x003FFFFF`
- `0xFFD00000` (kernel code/data) → physical `0x00100000`
- `0xFFD40000` (IDT) → physical `0x00140000`
- `0xFFD50000` (TSS) → physical `0x00150000`
- `0xFFD51000` (LDT area) → physical `0x00151000`

The kernel is linked as a flat binary at offset 0, so code and data use 0-based
offsets inside a segment whose base is `0xFFD00000`.

---

## Repository layout

```
MaluxOS/
├── Booter/                 # MBR / bootloader
│   ├── booterHDD.asm       # Hard-disk variant used by the makefile
│   ├── booterFDD.asm       # Floppy variant (DL = 0)
│   └── booter.asm          # Older/unused variant
├── CharLib/                # 8×8 bitmap font (cLib)
│   └── Chars.asm
├── FS_tables/              # On-disk filesystem tables
│   ├── FDT.asm             # File directory table
│   ├── FCDT.asm            # File control data table / sector chains
│   └── sat.asm             # Sector allocation table
├── Kernel/                 # 32-bit kernel
│   ├── main.c              # Kernel entry point
│   ├── Init/               # GDT, IDT, TSS, paging setup
│   ├── Interruptions/      # IRQ and exception handlers
│   ├── ProcessDispatcher/  # Scheduler and PCB switching
│   ├── sysCalls/           # Syscall entry points and C implementations
│   ├── Drivers/            # Keyboard, ATA, mouse scaffolding
│   ├── MemManager/         # Page bitmap allocator and page mapping
│   ├── FSystem/            # malFS implementation
│   ├── Makefile            # Build (i386 GCC + NASM + dd)
│   └── kernel              # Prebuilt flat kernel binary
├── Shell/                  # BUI.SYS (graphical shell + window manager)
│   ├── main.c
│   ├── lib.c
│   ├── lib.asm             # MLX_* user-space wrappers / MLX_Start
│   ├── malux.h             # User-space API header
│   └── EUI                 # Prebuilt shell binary
├── mal.pgm/                # Sample user program
│   ├── main.c
│   ├── lib.c
│   ├── lib.asm
│   └── malux.h
└── malux005.img            # Bootable 1.44 MB disk image
```

The `.o` files and prebuilt binaries are committed to the repository. They are
ELF32 i386 / raw flat binaries produced by the original toolchain.

---

## Building

### Historical toolchain (2014)

The makefiles assume:

- a native **i386** GCC toolchain (`gcc` / `ld` defaulting to 32-bit x86)
- **NASM** (`nasm -f elf`)
- the image at `../malux005.img` (override with
  `make IMG=/path/to/malux005.img`)

The original 2014 build used a custom `fwriter` tool. It has been replaced with
standard `dd`:

```sh
dd if=<file> of=<image> bs=512 seek=<lba> count=<sectors> conv=notrunc
```

`status=none` is a GNU coreutils extension; on other systems, drop it or redirect
`dd`'s output.

The historical build order is roughly:

```sh
cd Booter    && make    # write the MBR to LBA 0
cd ../Kernel && make    # build and write the kernel to LBA 1
cd ../CharLib && make   # write the font to LBA 69
cd ../FS_tables && make # write FDT/FCDT/SAT
cd ../Shell  && make    # build and write BUI.SYS (EUI) to LBA 100
cd ../mal.pgm && make   # build and write mal.pgm to LBA 119
```

### Modern hosts (recommended direction)

The in-tree makefiles **do not fully work as-is** on modern x86-64 or ARM64
hosts:

- Bare `gcc` produces 64-bit (x86-64) or AArch64 objects, while NASM produces
  ELF32 i386 objects.
- There is no `-m32`, `-m elf_i386`, `-ffreestanding`, `-fno-pie` or
  `-fno-pic` in the makefiles.

Image writes already use `dd`; make sure `../malux005.img` exists.

A modern build needs, at minimum:

- `i686-linux-gnu-gcc` and `i686-linux-gnu-ld` (or `gcc -m32` + `ld -m elf_i386`)
- `nasm -f elf32`
- freestanding flags:
  `-m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector`
  `-fno-asynchronous-unwind-tables -fno-builtin`
- `ld -m elf_i386 -z noseparate-code --oformat binary -Ttext 0`
- A linker script or explicit entry stub to guarantee the kernel entry point is
  at offset 0 (`-e main` is meaningless for `--oformat binary`)

On Debian/Ubuntu these packages are:

```sh
sudo apt install gcc-i686-linux-gnu binutils-i686-linux-gnu nasm qemu-system-x86
```

---

## Filesystem (malFS)

malFS is a simple sector-based filesystem. All sectors are 512 bytes and are
addressed by LBA.

### On-disk structures

**FDT (File Directory Table)** — 60-byte entries:

| Offset | Size | Field |
|---|---|---|
| 0 | 1 | properties |
| 1 | 1 | access |
| 2 | 1 | owner ID |
| 3 | 1 | type (`1` text, `2` program, `3` setting, `4` system) |
| 4 | 4 | size in bytes |
| 8 | 16 | password |
| 24 | 4 | first FCDT sector |
| 28 | 4 | FCDT offset |
| 32 | 28 | file name |

**FCDT (File Control Data Table)** — 12-byte chain entries:

| Offset | Size | Field |
|---|---|---|
| 0 | 4 | data sector number |
| 4 | 4 | next FCDT sector |
| 8 | 4 | next FCDT offset |

**SAT (Sector Allocation Table)** — first four dwords:

| Offset | Field |
|---|---|
| 0 | `0x0000FFFF` sentinel |
| 4 | next sector counter |
| 8 | number of free sectors |
| 12 | current FCDT sector |

### Intended sector layout

| LBA | Sectors | Contents |
|---|---|---|
| 0 | 1 | MBR (`Booter/booterHDD`) |
| 1–64 | 64 | Kernel (24,856 bytes used; 64 sectors reserved) |
| 69–70 | 2 | Character font `cLib` |
| 95 | 1 | FDT |
| 96–98 | 3 | Root directory (as read by `InitFS`) |
| 99 | 1 | FCDT |
| 100–118 | 19 | `BUI.SYS` / shell (`Shell/EUI`) |
| 119–128 | 10 | `mal.pgm` |
| 189 | 1 | SAT |
| 190 | 1 | SAT write-back target used by `AllocASector` |
| 200+ | — | Allocatable data sectors |

> The exact layout is defined by the makefiles plus `Kernel/FSystem/malFS.c`.
> Some sector references in the source are off by one; see
> [Known issues](#known-issues-and-limitations).

---

## Syscall interface

User programs invoke the kernel with software interrupts. The gates are
installed in `Kernel/Init/Init.c` (`Installer`) and the entry points live in
`Kernel/sysCalls/sysCallsLib.asm`.

| Interrupt | Service |
|---|---|
| `0x0C` | Stack-segment fault handler |
| `0x0D` | General-protection fault handler |
| `0x0E` | Page-fault handler |
| `0x20` | PIT timer (scheduler) |
| `0x21` | Keyboard |
| `0x2E` | ATA / hard-disk interrupt |
| `0x80` | Start the EUI/BUI process |
| `0xA0` | `PortOut` |
| `0xA1` | `PortIn` |
| `0xA2` | Read keyboard buffer |
| `0xA3` | Get time (stub) |
| `0xA4` | Print a character |
| `0xA5` | List files |
| `0xA6` | Set pixel |
| `0xA7` | Clear screen |
| `0xA8` | Draw rectangle |
| `0xAF` | Get mouse (stub) |
| `0xB2` | Quit process |
| `0xB3` | Allocate a page |
| `0xB4` | Execute a program |
| `0xB5` | Create a thread |
| `0xB6` | Destroy a thread |
| `0xB7` | Get process thread count |
| `0xB8` | Send message |
| `0xB9` | Get message |
| `0xBA` | Free a page |
| `0xC1` | Enter graphics mode (stub) |
| `0xC2` | Shutdown (stub: `cli; jmp $`) |
| `0xD0` | Create file |
| `0xD1` | Delete file |
| `0xD2` | Open file |
| `0xD3` | Close file |
| `0xD4` | Read file |
| `0xD5` | Write file |

---

## Shell commands

The shell (`Shell/main.c`, installed as `BUI.SYS`) supports:

| Command | Description |
|---|---|
| `cls` | Clear the screen |
| `ls` | List files |
| `ar <file>` | Execute a program |
| `<file>` | Execute a program (default action) |
| `mk <file>` | Create a text file |
| `rm <file>` | Delete a file |
| `cp <src> <dst>` | Copy a file |
| `fi <file>` | Show file information |

---

## Writing a user program

User programs are flat 32-bit binaries linked at offset 0. The entry point is
`MLX_Start` (in `Shell/lib.asm`), which calls your `MlxMain()` function.

```c
#include "malux.h"

WND *win;

int MlxMain(void)
{
    win = (WND *)MLX_MAlloc();
    MLX_CreateWindow(win, 100, 100, 220, 100, 0xFFFFFF, "Hello");
    MLX_ShowWindow(win);
    MLX_TextOut(win, 10, 10, "Hello from MaluxOS!", 0x000000);
    MLX_Pause();
    return 0;
}
```

- Copy `malux.h` from `Shell/` or `mal.pgm/`.
- Link with the user-space wrapper assembly (`lib.asm`) that provides
  `MLX_Start`, the `MLX_*` syscall stubs and `MlxMain` dispatch.
- The historical link command looks like:

  ```sh
  ld -e MLX_Start -Ttext 0 -Tbss <bss-address> --oformat binary \
     ASM.o main.o C.o -o <program>
  ```

- The kernel loads programs through `Sys_C_Execute` and maps them in the
  process address space starting at virtual `0x00400000` (4 MB).

---

## Known issues and limitations

**Build system**

- Requires a 32-bit x86 toolchain; the makefiles use bare `gcc`/`ld` with no
  `-m32` or `-m elf_i386`.
- The makefiles write to `../malux005.img`; ensure the image exists, or override
  the path with `make IMG=/path/to/malux005.img`.
- Prebuilt `.o` files and binaries are committed; there is no `clean` target.

**Boot / runtime**

- The bootloader performs a single 64-sector CHS read and ignores the BIOS
  carry/`AH` error status.
- It assumes BIOS drive `0x80` and CHS `(0,0,2)`; there is no LBA (`int 0x13,
  AH=0x42`) or E820 memory-map path.
- Legacy BIOS only: SeaBIOS plus a VBE-capable `-vga std`/`cirrus` is required.
- ATA PIO primary-master only (`0x1F0`, drive `0xA0`, IRQ14). No AHCI, virtio or
  NVMe.
- Fixed physical layout: page directory/tables, IDT, TSS, LDT, GDT, page bitmap
  and kernel stack all live at hard-coded addresses.
- The kernel must stay below 64 KB for the IDT gate offsets, and `.bss` is
  linked at `0xF000`.
- The physical page bitmap at `0xD60000` is inside the region the allocator can
  hand out (`ClearMap` reserves only the first 10 MB).
- The keyboard scan-code table appears to use Set-1 **break** codes
  (`make | 0x80`), while the driver reads make codes. Keyboard input may be
  inverted or unreliable.
- The mouse driver is mostly scaffolding (`ActivateMouse` is not wired into
  `Installer`).
- `Sys_Shutdown` is a stub (`cli; jmp $`); there is no ACPI shutdown.
- There is no serial console; debugging is VGA-only.
- Idle paths busy-wait (`while (1);`) instead of using `hlt`.
- 32-bit only, single CPU, legacy PIC/PIT, no APIC, no PAE, no SMP.

**Filesystem / image**

- `CharLib/cLib` is 1,528 bytes but the makefile writes only 2 sectors (1,024
  bytes), and the kernel reads LBA 70–71 while the font is written at LBA 69.
- The FDT references FCDT LBA 100, but `FS_tables/makefile` writes it at 99 and
  `Shell/EUI` is written at 100.
- `InitFS` reads the root directory at LBA 96; the committed image contains
  unrelated text in that region.
- `AllocASector` writes the SAT to LBA 190, while `FS_tables/makefile` writes it
  at LBA 189.
- The committed MBR differs from the current `Booter/booterHDD` source by four
  bytes at offset `0x1B8`.

---

## License

No license file is included in this repository. Unless a license is added,
treat the code as all rights reserved by the original author.

If you are the author, consider adding an explicit license (for example MIT or
GPL-2.0) so that others can study, use and redistribute the project.

---

## Credits

- Author: michael lou `<malpower@ymail.com>`
- Original repository: <https://github.com/malpower/MaluxOS>
- Initial commit: 2014-01-17
