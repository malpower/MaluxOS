extern void MLX_API INT_GP();					//list.asm
extern void MLX_API INT_CLOCK();				//list.asm
extern void MLX_API INT_SEUI();					//list.asm
extern void MLX_API INT_SS();					//list.asm

extern void MLX_API PrintString(char*,long);			//support.c
extern void MLX_API Cls();					//support.c

extern void MLX_API CreatePageInfo(DWORD mSize);			//InitLib.asm
extern void MLX_API CopyToNewGDTArea(unsigned char val,DWORD addr);	//InitLib.asm
extern void MLX_API ReloadGDT(DWORD gdtID);				//InitLib.asm
extern void MLX_API CreateIDT();				//InitLib.asm
extern void MLX_API CreateTSS();				//InitLib.asm
extern void MLX_API InstallFunctionToInterruption(DWORD intNum,void *fnAddr);	//InitLib.asm
extern void MLX_API LoadIDT();					//InitLib.asm
extern void MLX_API InitPCBs();					//InitLib.asm
extern void MLX_API LoadTSS();					//InitLib.asm
extern short MLX_API ActivateMouse();				//InitLib.asm

extern void MLX_API Sys_PortOut();				//syscallsLib.asm
extern void MLX_API Sys_PortIn();				//syscallsLib.asm
extern void MLX_API Sys_CreateThread();				//syscallsLib.asm
extern void MLX_API Sys_GetTime();				//syscallsLib.asm
extern void MLX_API Sys_DestroyThread();			//syscallsLib.asm

extern void MLX_API Driver_KeyBoard();				//driverslib.asm
extern void MLX_API Driver_Mouse();

extern void MLX_API SetKBFront(unsigned char f);
extern void MLX_API SetKBRear(unsigned char r);
extern void MLX_API Sys_ReadKeyBuff();
extern void MLX_API Sys_GetInGmode();
extern void MLX_API Sys_Shutdown();
extern void MLX_API Sys_SendMessage();
extern void MLX_API Sys_GetMessage();
extern void MLX_API Sys_QuitProcess();
extern void MLX_API Sys_CreateFile();
extern void MLX_API Sys_DeleteFile();
extern void MLX_API Sys_OpenFile();
extern void MLX_API Sys_ReadFile();
extern void MLX_API Sys_WriteFile();
extern void MLX_API Sys_CloseFile();
extern void MLX_API Sys_FreeAPage();
extern void MLX_API Sys_SetPixel();
extern void MLX_API Sys_ClearScreen();
extern void MLX_API Sys_DrawRect();
extern void MLX_API Sys_GetMouse();
extern void MLX_API Sys_PrintAChar();
extern void MLX_API ClearMap();
extern void MLX_API FreeAPage(DWORD *addr);
extern DWORD MLX_API *MallocAPage();
extern void MLX_API Driver_HD_Interruption();
extern void MLX_API KRNL_HD_ReadSector(DWORD sLow,DWORD sMid,DWORD sHght,DWORD *addr);
extern void MLX_API KRNL_HD_WriteSector(DWORD sLow,DWORD sMid,DWORD sHght,DWORD *addr);
extern void MLX_API Driver_HD_InitHDEnv();
extern unsigned int MLX_API GetPWNumber();
extern DWORD MLX_API ReadFromRealMem(DWORD *addr);
extern void MLX_API INT_PF();
extern char MLX_API *KRNL_GetTime();
extern void MLX_API InitFS();
extern void MLX_API PrintNumberAsHex(DWORD val,DWORD pos);
extern void MLX_API Sys_Execute();
extern void MLX_API Sys_AllocAPage();
extern void MLX_API Sys_GetProcessThreads();
extern void MLX_API Sys_ListFile();
extern void MLX_API X_TEST();



typedef struct _strct_GD																		//global discription
{
	unsigned int lenLow;
	unsigned int baseLow;
	unsigned char baseMiddle;
	unsigned int attr;
	unsigned char baseHigh;
}GD,*pGD;
