extern void MLX_API PrintString(char *val,long pos);				//保护模式下输出字符串,support.c
extern void MLX_API Cls();								//保护模式下清屏函数,support.c
extern void MLX_API PrintNumberAsHex(DWORD val,long pos);//保护模式下输出DDWORD,support.c
extern void MLX_API SystemStop();						//support.c
extern DWORD MLX_API InitMem();					//初始化内存环境,Init/Init.c
extern DWORD MLX_API InitInt();					//Init/Init.c
extern DWORD MLX_API Installer();				//Init/Init.c
extern DWORD MLX_API InitTSS();					//Init/Init.c
extern void MLX_API GetInUI();							//InitLib.asm
extern void MLX_API X_TEST();
extern void MLX_API CreateProcess(DWORD ldtID,DWORD *pAddr,DWORD sPointer);
extern DWORD MLX_API *MallocAPage();
extern void GT();







#define MEM_ERROR_UNKNOW 1
#define INT_ERROR_UNKNOW 1
