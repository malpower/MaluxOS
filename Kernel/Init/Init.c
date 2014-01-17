#include "../os.h"
#include "Init.h"


DWORD *gRam;

void MLX_API WriteGdtItem(DWORD low,DWORD high,unsigned int num)						//Write new gdt datas into address from 0 on.
{
	char *p;
	p=(char*)&low;
	unsigned int i=num*8;
	for (;i<num*8+8;i++)
	{
		CopyToNewGDTArea(*p,i);
		p++;
	}
}

void MLX_API InitSeg()
{
	DWORD i;
	WriteGdtItem(0x0000FFFF,0x00409800,0);
	WriteGdtItem(0x8000FFFF,0x0040F20B,1);							//for video
	WriteGdtItem(0x0000FFFF,0xFF4098D0,2);							//for kernel
	WriteGdtItem(0x0000FFFF,0xFF4092D0,3);  						//for kernel DATA
	WriteGdtItem(0x0000FFFF,0xFF4092D4,4);							//for IDT use
	WriteGdtItem(0x00000FFF,0xFF4089D5,5);							//TSS for the first CPU
	WriteGdtItem(0x00000FFF,0xFF4092D5,6);							//TSS data describing.
	WriteGdtItem(0x1000EFFF,0xFF4092D5,7);							//LDT data describing.
	WriteGdtItem(0x0000FFFF,0x00CF9200,8);							//0~4G ,for system use.
	WriteGdtItem(0x00000100,0x00C092D6,9);							//Memory map.for mem manager to use.
	for (i=0;i<31;i++)												//LDT area,storing ldts for all processes
	{
		WriteGdtItem(0x100000FF+((i<<16)*0x100),0x0040E215,i+10);
	}
	ReloadGDT(i+10);
}

void MLX_API InitPage()								//Initialize Page informations
{
	CreatePageInfo(0xFFFFFFFF/1024);
}

DWORD MLX_API InitMem()							//initialize memory,include Pages and segments.
{
	InitPage();
	InitSeg();
	return 0;
}

DWORD MLX_API InitInt()							//initialize interruption system
{
	CreateIDT();
	return 0;
}

DWORD MLX_API InitTSS()							//initialize tss.
{
	CreateTSS();
	LoadTSS();
	return 0;
}

void MLX_API InstallProcessDispatcher() 					//install process dispatcher onto a interruption.
{
	InitPCBs();
	InstallFunctionToInterruption(32,(void*)INT_CLOCK);
}

void MLX_API InstallChrDrivers()							//initialize Charactor drivers.
{
	SetKBRear(1);
	SetKBFront(0);
	InstallFunctionToInterruption(33,(void*)Driver_KeyBoard);
	gRam=(DWORD*)ReadFromRealMem((DWORD*)0x00090000+10);
}

void MLX_API InstallBlkDrivers()							//blk driver initializing.
{
	Driver_HD_InitHDEnv();
	InstallFunctionToInterruption(46,(void*)Driver_HD_Interruption);
}

void MLX_API InstallMemManager()							//memory manager initializing.incomplete.
{
	ClearMap();
}

void MLX_API InstallMouseDrivers()
{

	InstallFunctionToInterruption(44,(void*)Driver_Mouse);
}


DWORD MLX_API Installer()						//install some things to right places.
{
	InstallFunctionToInterruption(0x0C,(void*)INT_SS);
	InstallFunctionToInterruption(0x0D,(void*)INT_GP);
	InstallFunctionToInterruption(0x0E,(void*)INT_PF);

	InstallFunctionToInterruption(0x80,(void*)INT_SEUI);

	InstallFunctionToInterruption(0xA0,(void*)Sys_PortOut);
	InstallFunctionToInterruption(0xA1,(void*)Sys_PortIn);
	InstallFunctionToInterruption(0xA2,(void*)Sys_ReadKeyBuff);
	InstallFunctionToInterruption(0xA3,(void*)Sys_GetTime);
	InstallFunctionToInterruption(0xA4,(void*)Sys_PrintAChar);
	InstallFunctionToInterruption(0xA5,(void*)Sys_ListFile);
	InstallFunctionToInterruption(0xA6,(void*)Sys_SetPixel);
	InstallFunctionToInterruption(0xA7,(void*)Sys_ClearScreen);
	InstallFunctionToInterruption(0xA8,(void*)Sys_DrawRect);
	InstallFunctionToInterruption(0xAF,(void*)Sys_GetMouse);

	InstallFunctionToInterruption(0xB2,(void*)Sys_QuitProcess);
	InstallFunctionToInterruption(0xB3,(void*)Sys_AllocAPage);
	InstallFunctionToInterruption(0xB4,(void*)Sys_Execute);
	InstallFunctionToInterruption(0xB5,(void*)Sys_CreateThread);
	InstallFunctionToInterruption(0xB6,(void*)Sys_DestroyThread);
	InstallFunctionToInterruption(0xB7,(void*)Sys_GetProcessThreads);
	InstallFunctionToInterruption(0xB8,(void*)Sys_SendMessage);
	InstallFunctionToInterruption(0xB9,(void*)Sys_GetMessage);
	InstallFunctionToInterruption(0xBA,(void*)Sys_FreeAPage);

	InstallFunctionToInterruption(0xC1,(void*)Sys_GetInGmode);
	InstallFunctionToInterruption(0xC2,(void*)Sys_Shutdown);

	InstallFunctionToInterruption(0xD0,(void*)Sys_CreateFile);
	InstallFunctionToInterruption(0xD1,(void*)Sys_DeleteFile);
	InstallFunctionToInterruption(0xD2,(void*)Sys_OpenFile);
	InstallFunctionToInterruption(0xD3,(void*)Sys_CloseFile);
	InstallFunctionToInterruption(0xD4,(void*)Sys_ReadFile);
	InstallFunctionToInterruption(0xD5,(void*)Sys_WriteFile);

	InstallMemManager();
	InstallChrDrivers();
	InstallBlkDrivers();
	InstallProcessDispatcher();
	LoadIDT();
	InitFS();
	return 0;
}


