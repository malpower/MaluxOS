
extern void MLX_API KRNL_HD_ReadSector(DWORD sLow,DWORD sMid,DWORD sHght,DWORD *addr);
extern void MLX_API KRNL_HD_WriteSector(DWORD sLow,DWORD sMid,DWORD sHght,DWORD *addr);
extern DWORD MLX_API *MallocAPage();
extern void MLX_API FreeAPage(DWORD *addr);
extern void MLX_API WriteToRealMem(DWORD *addr,DWORD val);
extern DWORD MLX_API ReadFromRealMem(DWORD *addr);
extern void MLX_API PrintNumberAsHex(DWORD val,DWORD pos);
extern void MLX_API PrintString(char *val,DWORD pos);
extern int MLX_API StrCmp(const char *a,const char *b);
extern void MLX_API Wbrm(char *addr,char val);





