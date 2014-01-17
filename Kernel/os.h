#define MLX_API __attribute__((cdecl))				//macro MLX_API
typedef unsigned int DWORD;							//DWORD data type,as a int on x86,32bits.


extern void MLX_API PrintA(char val,long pos);					//PM模式下输出字符,lib.asm
extern int MLX_API GetMemSize();							//获取booter得到的系统扩展内存大小,lib.asm
extern void MLX_API Die();								//死循环函数,lib.asm



typedef struct _FILE						//FILE data structure,used for controlling files.
{
	unsigned char properties;
	unsigned char access;
	unsigned char ownerID;
	unsigned char type;
	DWORD size;
	char password[16];
	DWORD secNumOfFCD;
	DWORD offsetOfFCD;
	char name[28];
	DWORD *dir;
}FILE;

typedef struct _PCB
{
	DWORD eip;
	DWORD cs;
	DWORD eflags;
	DWORD esp;
	DWORD ss;
	DWORD ebp;
	DWORD ecx;
	DWORD edx;
	DWORD edi;
	DWORD esi;
	DWORD ds;
	DWORD es;
	DWORD eax;
	DWORD ebx;
	DWORD gs;
	DWORD fs;
	DWORD ldt;
	DWORD status;
	DWORD priority;
	DWORD nextp;
	DWORD pid;
	DWORD cr3;
	DWORD msgQueueAddr;
	DWORD msgQueueLength;
	char processName[28];
}PCB;
#define TSK_WATING 3
#define TSK_RUNNING 2
#define TSK_READY 1
#define TSK_NONE 0
#define TSK_DESTROYING 100



#define LINE(x) (x<<16)					//Position calculating.
#define POS(x,y) ((y<<16)+x)							//

#define ADDRESS_PCB_SWAP 0x00008000				//address of PCB swap area.
#define ADDRESS_PCB_LIST 0x00008100				//address of PCB list.
#define SIZE_OF_PCB 0x100						//size of PCB


#define PG_RW 0x27								//Memory page attributes.
#define PG_NA 0x21
#define PG_R  0x25
