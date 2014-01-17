extern char MLX_API PortIn(DWORD port);
extern void MLX_API PrintString(char *val,long pos);								//保护模式下输出字符串,support.c
extern void MLX_API PrintNumberAsHex(DWORD val,long pos);				//保护模式下输出字符串,support.c
extern unsigned char MLX_API ReadKBFront();
extern unsigned char MLX_API ReadKBRear();
extern void MLX_API SetKBFront(unsigned char f);
extern void MLX_API SetKBRear(unsigned char r);
extern char MLX_API GetAsciiCode(unsigned char scanCode,unsigned char isShift);
extern void MLX_API Driver_HD_ReadSector();
extern void MLX_API Driver_HD_WriteSector();


#define KEY_BUFF_ADDR 0x0000A000
