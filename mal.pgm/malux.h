
#define MLX_API __attribute__((cdecl))
#define LINE(x) (x<<16)
#define POS(y,x) ((x<<16)+y)
typedef unsigned int DWORD;

typedef struct
{
	DWORD msg;
	DWORD param;
}MSG;


typedef struct
{
	char name[28];
	DWORD unused;
	DWORD type;
	DWORD properties;
	DWORD access;
	DWORD size;
}*FILE;

typedef struct
{
	DWORD x;
	DWORD y;
	DWORD width;
	DWORD height;
	DWORD bgColor;
	char *title;
	DWORD check;
}WND;

#define INVALID_FILE 0

extern void MLX_API MLX_PrintString(char *str,int pos,DWORD color,DWORD bgColor);
extern void MLX_API MLX_CreateThread(void(*fn));
extern void MLX_API MLX_PrintChar(char letter,int pos,DWORD color,DWORD bgColor);
extern int MLX_API MLX_ReadKeyBuff();
extern char MLX_API MLX_PortIn(int port);
extern void MLX_API MLX_PortOut(char val,int port);
extern int MLX_API MLX_Execute(char *filename);
extern void MLX_API *MLX_MAlloc();
extern void MLX_API MLX_MFree(void *addr);
extern void MLX_API MLX_GetInput(char *buff,DWORD size,int pos,DWORD color,DWORD Bgcolor);
extern void MLX_API MLX_PrintNumberAsHex(DWORD val,DWORD pos,DWORD color,DWORD BgColor);
extern void MLX_API MLX_Pause();
extern void MLX_API MLX_ClearScreen(DWORD left,DWORD top,DWORD width,DWORD height);
extern int MLX_API MLX_StrCmp(const char *a,const char *b);
extern void MLX_API MLX_DiscardCPU();
extern void MLX_API MLX_Shutdown();
extern char MLX_API *MLX_ListFile();
extern int MLX_API MLX_WaitProcess(int pid);
extern void MLX_API MLX_SendMessage(DWORD pid,DWORD msgType,DWORD param);
extern int MLX_API MLX_GetMessage(MSG *msg);
extern int MLX_API MLX_CreateFile(const char *filename,DWORD type);
extern int MLX_API MLX_DeleteFile(const char *filename);
extern FILE MLX_API MLX_OpenFile(const char *filename);
extern void MLX_API MLX_CloseFile(FILE pf);
extern DWORD MLX_API MLX_ReadFile(FILE pf,void *buff,DWORD pos,DWORD len);
extern DWORD MLX_API MLX_WriteFile(FILE pf,void *buff,DWORD pos,DWORD len);
extern DWORD MLX_API MLX_ToDword(char *val);
extern void MLX_API MLX_DrawRect(DWORD left,DWORD top,DWORD width,DWORD height,DWORD color);
extern void MLX_API MLX_CreateWindow(WND *pWnd,DWORD left,DWORD top,DWORD width,DWORD height,DWORD bgColor,char *title);
extern void MLX_API MLX_ShowWindow(WND *pWnd);
extern void MLX_API MLX_HideWindow(WND *pWnd);
extern void MLX_API MLX_LightWindow(WND *pWnd);
extern void MLX_API MLX_DarkWindow(WND *pWnd);
extern void MLX_API MLX_TextOut(WND *pWnd,DWORD left,DWORD top,char *val,DWORD bgColor);
extern void MLX_API MLX_NumberOut(WND *pWnd,DWORD left,DWORD top,DWORD val,DWORD bgColor);










/*-------------------------Message defines--------------------------------------*/


#define MM_QUIT      0xFFFFFFFF
#define MM_NONE      0x00000000
#define MM_KEYDOWN   0x00000010
#define MM_PAINT     0x00000020
#define MM_SETFOCUS  0x00000030
#define MM_KILLFOCUS 0x00000031







