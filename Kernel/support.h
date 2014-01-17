#define ADDRESS_PCB_LIST 0x00008100
#define SIZE_OF_PCB 0x100


typedef struct _cTime
{
	unsigned char Seconds;
	unsigned char Minutes;
	unsigned char Hours;
	unsigned char Day;
	unsigned char Month;
	unsigned char Year;
	unsigned int noUse;									//make the size of this structure be 8 bytes.
}cTime;




extern void MLX_API WriteToRealMem(DWORD *addr,DWORD val);
extern void MLX_API KRNL_HD_ReadSector(DWORD sLow,DWORD sMid,DWORD sHght,DWORD *addr);
extern DWORD MLX_API *MallocAPage();
extern void MLX_API FreeAPage();
extern void MLX_API NP();
extern void MLX_API PortOut(unsigned char val,DWORD port);
extern unsigned char MLX_API PortIn(DWORD port);
extern DWORD MLX_API ReadFromRealMem(DWORD *addr);
extern DWORD MLX_API *LoadFile(FILE *pf);
extern int MLX_API GetFile(FILE *pf,int index,DWORD *dir);
extern void MLX_API CreateProcess(DWORD ldtID,DWORD *pAddr,DWORD sPointer);
extern void MLX_API CreateFile(FILE *pf);
extern DWORD MLX_API AllocASector();
extern int MLX_API OpenFile(FILE *pf,const char *filename,DWORD *dir);
extern int MLX_API WriteFile(FILE *pf,DWORD *buff,DWORD size);
extern void MLX_API UpdateFile(FILE *pf);
extern void MLX_API DeleteFile(FILE *pf);
extern void MLX_API X_TEST();
extern DWORD MLX_API *LinkPage(DWORD pid,DWORD *pMem,DWORD aMode);
extern void MLX_API Wbrm(char *addr,char val);





