#include "malux.h"

WND *mWin;


static void Cls(char *param)
{
	MLX_ShowWindow(mWin);
}

int MlxMain()
{
	char *a,*b,*c;
	char w=1;
	int i,k;
	int pid;
	DWORD fSize;
	char *fTypes[]={"NONE","Text file","Program file","Setting file","System File"};
	FILE f;
	mWin=(WND*)MLX_MAlloc();
	WND *fiWin=(WND*)MLX_MAlloc();
	c=(char*)MLX_MAlloc();
	MLX_MAlloc();
	a=(char*)MLX_MAlloc();
	MLX_MAlloc();
	MLX_CreateWindow(mWin,50,50,300,230,0x000000,"Console Window");
	MLX_ShowWindow(mWin);
	while (1)
	{
		a[0]=0;
		MLX_TextOut(mWin,0,0,"!CMD\\>",0xFFFFFF);
		MLX_GetInput(a,POS((mWin->x+56),(mWin->y+21)),1024,0xFFFFFF,0x000000);
		MLX_TextOut(mWin,56,0,"                         ",0xFFFFFF);
		if (!MLX_StrCmp(a,""))
		{
			continue;
		}
		else if (!MLX_StrCmp(a,"cls"))
		{
			Cls((char*)0);
		}
		else if (a[0]=='a' && a[1]=='r' && a[2]==' ')
		{
			for (i=0;i<25;i++)
			{
				a[i]=a[i+3];
			}
			pid=MLX_Execute(a);
			continue;
		}
		else if (a[0]=='m' && a[1]=='k' && a[2]==' ')
		{
			for (i=0;i<25;i++)
			{
				a[i]=a[i+3];
			}
			MLX_CreateFile(a,1);
			continue;
		}
		else if (a[0]=='r' && a[1]=='m' && a[2]==' ')
		{
			for (i=0;i<25;i++)
			{
				a[i]=a[i+3];
			}
			MLX_DeleteFile(a);
			continue;
		}
		else if (!MLX_StrCmp(a,"ls"))
		{
			b=MLX_ListFile();
			k=0;
			while (b[0])
			{
				for (i=0;i<6;i++)
				{
					if (b[0])
					{
						MLX_PrintString(b,POS((i*20+4)*9,k*10),0xFF0000,0xFFFFFF);
						b+=32;
					}
				}
				k++;
			}
			MLX_MFree((void*)b);
		}
		else if (a[0]=='c' && a[1]=='p' && a[2]==' ')
		{
			for (i=0;i<100;i++)
			{
				a[i]=a[i+3];
			}
			for (i=0;i<60;i++)
			{
				if (a[i]==32)
				{
					a[i]=0;
					break;
				}
			}
			if (i==60)
			{
				continue;
			}
			i++;
			f=MLX_OpenFile(a);
			if (f!=INVALID_FILE)
			{
				MLX_CreateFile(&a[i],f->type);
				fSize=f->size;
				MLX_ReadFile(f,(void*)c,0,fSize);
				f=MLX_OpenFile(&a[i]);
				fSize=MLX_WriteFile(f,(void*)c,0,fSize);
				MLX_CreateWindow(fiWin,200,200,200,50,0xFFFFFF,"File copy");
				MLX_ShowWindow(fiWin);
				MLX_TextOut(fiWin,5,5,"Copying finished.",0);
				MLX_NumberOut(fiWin,5,15,fSize,0);
				MLX_TextOut(fiWin,60,15,"bytes copied.",0);
			}
			MLX_Pause();
			MLX_HideWindow(fiWin);
			continue;
		}
		else if (a[0]=='f' && a[1]=='i' && a[2]==' ')
		{
			MLX_CreateWindow(fiWin,200,200,300,200,0xCCCCCC,"File Information");
			MLX_ShowWindow(fiWin);
			for (i=0;i<100;i++)
			{
				a[i]=a[i+3];
			}
			f=MLX_OpenFile(a);
			if (f!=INVALID_FILE)
			{
				MLX_TextOut(fiWin,5,5,"FILE:",0xFF0000);
				MLX_TextOut(fiWin,65,5,f->name,0xFF0000);
				MLX_TextOut(fiWin,5,15,"SIZE:",0xFF0000);
				MLX_NumberOut(fiWin,65,15,f->size,0xFF0000);
				MLX_TextOut(fiWin,5,25,"TYPE:",0xFF0000);
				MLX_TextOut(fiWin,65,25,fTypes[f->type],0xFF0000);
			}
			else
			{
				MLX_TextOut(fiWin,5,5,"Can't open file",0xFF0000);
			}
			MLX_Pause();
			MLX_HideWindow(fiWin);
			continue;
		}
		else
		{
			pid=MLX_Execute(a);
			if (!pid)
			{
				MLX_TextOut(mWin,0,0,"Bad command or filename!",0xFFFFFF);
				while (1)
				{
					if (MLX_ReadKeyBuff())
					{
						MLX_TextOut(mWin,0,0,"                                ",0xFFFFFF);
						break;
					}
				}
			}
			else
			{
				while (1)
				{
					if (MLX_WaitProcess(pid))
					{
						w=MLX_ReadKeyBuff();
						if (w=='!')
						{
							MLX_SendMessage(pid,MM_KILLFOCUS,0);
							MLX_SendMessage(pid,MM_PAINT,0);
							MLX_DiscardCPU();
							break;
						}
						if (w)
						{
							MLX_SendMessage(pid,MM_KEYDOWN,(DWORD)w);
						}
					}
					else
					{
                        Cls(0);
						break;
					}
					MLX_DiscardCPU();
				}
			}
		}
	}
	return 0;
}
