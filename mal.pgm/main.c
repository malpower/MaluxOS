#include "malux.h"


WND *mWin;

void abc()
{
	int x=0;
	while (x<1000)
	{
		MLX_NumberOut(mWin,0,0,x,0);
		x++;
	}
}

int MlxMain()
{
	mWin=(WND*)MLX_MAlloc();
	DWORD *x=(DWORD*)MLX_MAlloc();
	x+=1000;
	*x=100;
	MLX_CreateWindow(mWin,400,400,200,160,0xFFFFFF,"Test Window 002");
	MLX_ShowWindow(mWin);
	MLX_CreateThread(abc);
	MLX_Pause();
	return 0;
}

