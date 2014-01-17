extern void MLX_API PrintNumberAsHex(DWORD val,DWORD pos);					//LIB.asm
extern void MLX_API PrintString(char *val,DWORD pos);								//LIB.asm
extern unsigned char MLX_API ReadKBFront();
extern unsigned char MLX_API ReadKBRear();
extern void MLX_API SetKBFront(unsigned char f);
extern void MLX_API SetKBRear(unsigned char r);
extern DWORD *GetFreePCB();
extern DWORD GetFreeLDT();
extern DWORD MLX_API ReadFromRealMem(DWORD *addr);
extern DWORD MLX_API WriteToRealMem(DWORD *addr,DWORD val);
extern DWORD MLX_API *LoadFile(FILE *pf);
extern int MLX_API GetFile(FILE *pf,int index,DWORD *dir);
extern void MLX_API CreateProcess(DWORD ldtID,DWORD *pAddr,DWORD sPointer);
extern void MLX_API CreateFile(FILE *pf);
extern int MLX_API OpenFile(FILE *pf,const char *filename,DWORD *dir);
extern int MLX_API WriteFile(FILE *pf,DWORD *buff,DWORD size);
extern void MLX_API UpdateFile(FILE *pf);
extern void MLX_API DeleteFile(FILE *pf);
extern void MLX_API *GetCurrentPCB();
extern char MLX_API *GetPCB(DWORD pid);
extern void MLX_API FreeAPage(DWORD *addr);
extern void MLX_API FreePageTree(DWORD cr3);
extern DWORD* MLX_API MallocAPage();
extern void MLX_API StrCpy(char *dst,const char *src);
extern DWORD MLX_API *LinkPage(DWORD pid,DWORD *pMem,DWORD aMode);
extern void MLX_API UnlinkPage(DWORD lMem);
extern DWORD MLX_API *GetPAddr(DWORD lMem);
extern char MLX_API Rbrm(char *addr);
extern void MLX_API Wbrm(char *addr,char val);
extern void MLX_API DrawChar(char val,DWORD pos,DWORD color,DWORD bgColor);
extern void MLX_API DrawString(char *val,DWORD pos);
extern void MLX_API DrawNumberAsHex(DWORD val,DWORD pos);
extern void MLX_API DrawPixel(DWORD left,DWORD top,DWORD color);
extern void MLX_API ExitAllThreads(DWORD pid);




extern int newPID;


