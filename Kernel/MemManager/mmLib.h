extern char MLX_API ReadMMap(const char *addr);
extern void MLX_API WriteMMap(char *addr,unsigned char val);
extern void MLX_API PrintNumberAsHex(DWORD val,DWORD pos);
extern DWORD MLX_API *GetCurrentPCB();
extern DWORD MLX_API *GetPCB(DWORD pcbID);
extern DWORD MLX_API ReadFromRealMem(DWORD *addr);
extern void MLX_API WriteToRealMem(DWORD *addr,DWORD val);
extern void MLX_API DrawNumberAsHex(DWORD val,DWORD pos);

#define SIZE_OF_PAGE 4096
