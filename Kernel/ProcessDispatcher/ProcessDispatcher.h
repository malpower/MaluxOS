extern void MLX_API PT();
extern void MLX_API ReloadLDT(DWORD ldt);
extern void MLX_API LoadCr3(DWORD cr3);																//ProcessDispatcherLib.asm
extern void MLX_API PrintNumberAsHex(DWORD val,long pos);								//support.c
extern void MLX_API ReloadGDT(DWORD gdtID);		 							//InitLib.asm
extern void MLX_API FreePageTree(DWORD cr3);
extern void MLX_API ExitAllThreads(DWORD pid);
extern void MLX_API PrintString(const char *val,DWORD pos);




#define LEN_OF_PCB_LIST 30
#define SIZE_OF_PCB_SWAP 64
#define CS_SYSTEM 16

