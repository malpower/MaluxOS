#include "../os.h"
#include "sysCalls.h"

extern DWORD *cPcbAdrs;
extern DWORD *rootFS;
extern DWORD lenOfRootFS;

extern void MLX_API Sys_DrawRect(DWORD,DWORD,DWORD,DWORD,DWORD);

char MLX_API Sys_C_ReadKeyBuff()									//system call,read a char from keyboard buffer.programed in C.
{
	char *kb=(char*)0xA000;
	unsigned char f=ReadKBFront();
	unsigned char r=ReadKBRear();
	if ((f+1)!=r)
	{
		f++;
		SetKBFront(f);
		return kb[f];
	}
	else
	{
		return 0;
	}
	return 0;
}

void MLX_API Sys_C_GetMouse()
{
}

DWORD MLX_API *Sys_C_OpenFile(const char *fn)
{
	DWORD *tp=MallocAPage();
	FILE op;
	DWORD *cp,pid;
	DWORD *a=(DWORD*)fn;
	DWORD *addr;
	cp=(DWORD*)GetCurrentPCB();
	pid=*(cp+20);
	int i;
	for (i=0;i<7;i++)
	{
		WriteToRealMem(tp+i,*(a+i));
	}
	WriteToRealMem(tp+i,0);
	if (OpenFile(&op,fn,rootFS))
	{
		if (op.type==4)
		{
			return 0;
		}
		addr=LoadFile(&op);
		WriteToRealMem(tp+8,(DWORD)op.type);
		WriteToRealMem(tp+9,(DWORD)op.properties);
		WriteToRealMem(tp+10,(DWORD)op.access);
		WriteToRealMem(tp+11,(DWORD)op.size);
		WriteToRealMem(tp+12,(DWORD)tp);
		WriteToRealMem(tp+13,(DWORD)addr);
		a=LinkPage(pid,tp,PG_R);
		return a;
	}
	return 0;
}


void MLX_API Sys_C_CloseFile(DWORD *p)
{
	DWORD *addr,*aList,*cr3,*pte;
	int i,j,k;
	addr=(DWORD*)ReadFromRealMem(p+13);
	while (1)
	{
		aList=(DWORD*)ReadFromRealMem(addr);
		if (!aList)
		{
			break;
		}
		FreeAPage(aList);
		addr++;
	}
	FreeAPage(addr);
	cr3=GetCurrentPCB();
	i=*(cr3+21);
	i>>=12;
	i<<=12;
	cr3=(DWORD*)i;
	for (i=1;i<1024;i++)
	{
		k=ReadFromRealMem(cr3+i);
		k>>=12;
		k<<=12;
		pte=(DWORD*)k;
		for (j=0;j<1024;j++)
		{
			if (ReadFromRealMem(pte)==(DWORD)p)
			{
				WriteToRealMem(pte,0);
				FreeAPage(p);
				return;
			}
		}
	}
}


DWORD MLX_API Sys_C_ReadFile(DWORD *file,char *dst,DWORD sp,DWORD len)
{
	int i=0,s=0;
	char *p1,*p2;
	DWORD* fmap=(DWORD*)ReadFromRealMem(file+13);
	p1=(char*)GetPAddr((DWORD)dst);
	if (sp+len>ReadFromRealMem(file+11))
	{
		return 0;
	}
	while (sp>=0x00001000)
	{
		i++;
		sp-=0x00001000;
	}
	p2=(char*)ReadFromRealMem(fmap+i);
	p2+=sp;
	while (len)
	{
		Wbrm(p1,Rbrm(p2));
		p2++;
		p1++;
		if ((int)p2%0x00001000==0 && p2!=0)
		{
			i++;
			p2=(char*)ReadFromRealMem(fmap+i);
		}
		len--;
		s++;
	}
	return s;
}

DWORD MLX_API Sys_C_WriteFile(DWORD *file,char *src,DWORD sp,DWORD len)
{
	FILE tar;
	DWORD* fn=(DWORD*)0x0000A000;
	int i=0,s=0;
	char *p1,*p2;
	DWORD* fmap=(DWORD*)ReadFromRealMem(file+13);
	p1=(char*)GetPAddr((DWORD)src);
	for (i=0;i<8;i++)
	{
		fn[i]=ReadFromRealMem(file+i);
	}
	i=0;
	while (sp>=0x00001000)
	{
		i++;
		sp-=0x00001000;
	}
	p2=(char*)ReadFromRealMem(fmap+i);
	p2+=sp;
	while (len)
	{
		Wbrm(p2,Rbrm(p1));
		p2++;
		p1++;
		if ((int)p2%0x00001000==0 && p2!=0)
		{
			i++;
			p2=(char*)ReadFromRealMem(fmap+i);
			if (!p2)
			{
				p2=(char*)MallocAPage();
				WriteToRealMem(fmap+i,(DWORD)p2);
				WriteToRealMem(fmap+i+1,0);
			}
		}
		len--;
		s++;
	}
	if (OpenFile(&tar,(char*)fn,rootFS))
	{
		WriteFile(&tar,fmap,s);
	}
	return s;
}


DWORD MLX_API Sys_C_CreateFile(char *fn,DWORD type)
{
	FILE t;
	if (OpenFile(&t,fn,rootFS))
	{
		return 0;
	}
	fn[27]=0;
	StrCpy((char*)t.name,fn);
	t.type=type;
	t.password[0]=0;
	t.size=0;
	t.secNumOfFCD=0;
	t.offsetOfFCD=0;
	t.dir=rootFS;
	CreateFile(&t);
	return 1;
}



DWORD MLX_API Sys_C_DeleteFile(char *fn)
{
	FILE t;
	fn[27]=0;
	if (OpenFile(&t,fn,rootFS))
	{
		DeleteFile(&t);
	}
	return 0;
}

void MLX_API Sys_C_QuitProcess()
{
    DWORD *x=GetCurrentPCB();
	FreePageTree(*(x+21));
	FreeAPage((DWORD*)*(x+22));
    ExitAllThreads((DWORD)*(x+20));
}

void MLX_API Sys_C_DestroyThread()
{
	int i;
	char *n=(char*)ADDRESS_PCB_LIST;
	char *p=(char*)ADDRESS_PCB_LIST;
	for (i=0;i<31;i++)
	{
		if (*(n+68)==TSK_RUNNING)
		{
			break;
		}
		n+=0x100;
	}
	for (i=0;i<31;i++)
	{
		if (*(p+76)==((DWORD)n-ADDRESS_PCB_LIST)/0x100)
		{
			break;
		}
		p+=0x100;
	}
	*(p+76)=*(n+76);
	*(n+68)=TSK_DESTROYING;
}


DWORD MLX_API Sys_C_CreateThread(char *address,DWORD esp)		//system call,create a thread,programed in C.
{
	char *nPCB=(char*)ADDRESS_PCB_LIST+0x100;
	char *cPCB=(char*)ADDRESS_PCB_LIST;
	DWORD *p;
	DWORD i;
	DWORD nextP,newP;
	for (i=1;i<31;i++)
	{
		if ((DWORD)*(nPCB+17*4)==TSK_NONE)
		{
			*(nPCB+17*4)=TSK_READY;
			break;
		}
		nPCB=nPCB+SIZE_OF_PCB;
	}
	if (i==30 && (DWORD)*(nPCB+17*4)!=TSK_NONE)
	{
		return 1;
	}
	newP=i;
	for (i=0;i<31;i++)
	{
		if ((DWORD)*(cPCB+17*4)==TSK_RUNNING)
		{
			break;
		}
		cPCB=cPCB+SIZE_OF_PCB;
	}
	nextP=*(cPCB+19*4);
	for (i=0;i<100;i++)
	{
		nPCB[i]=cPCB[i];
	}
	p=(DWORD*)nPCB;
	*p=(DWORD)address;
	*(p+17)=TSK_READY;
	*(p+3)=esp;
	*(cPCB+19*4)=newP;
	*(nPCB+19*4)=nextP;
	return 0;
}

int MLX_API Sys_C_Execute(char *fname)
{
	FILE exe;
	DWORD nLdt;
	int i=1;
	DWORD esp;
	DWORD *addr;
    PCB *thisPcb;
	extern DWORD *rootFS;
	if (OpenFile(&exe,fname,rootFS))
	{
		if (exe.type==2)
		{
			nLdt=GetFreeLDT();
			if (nLdt<0x1E00 && nLdt!=0)
			{
				addr=LoadFile(&exe);
				CreateProcess(nLdt,(DWORD*)ReadFromRealMem(addr),0xFF0);
				while (ReadFromRealMem(addr+i))
				{
					LinkPage(newPID-1,(DWORD*)ReadFromRealMem(addr+i),PG_RW);
					i++;
				}
				FreeAPage(addr);
				addr=MallocAPage();
				esp=(DWORD)LinkPage(newPID-1,addr,PG_RW);
				esp+=4080;
				addr=(DWORD*)GetPCB(newPID-1);
				*(addr+3)=esp;
                thisPcb=(PCB*)addr;
                for (i=0;i<28;i++)
                {
                    thisPcb->processName[i]=fname[i];
                }
                    thisPcb->processName[27]=0;
				return newPID-1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}

DWORD MLX_API Sys_C_LinkPage(DWORD *pMem)
{
	DWORD *cp,pid;
	cp=(DWORD*)GetCurrentPCB();
	pid=*(cp+20);
	return (DWORD)LinkPage(pid,pMem,PG_RW);
}

DWORD MLX_API *Sys_C_ListFile()
{
	extern DWORD *rootFS;
	extern DWORD lenOfRootFS;
	DWORD *p;
	char *s=(char*)MallocAPage();
	p=(DWORD*)Sys_C_LinkPage((DWORD*)s);
	int i,j=0,k;
	FILE f;
	for (i=0;i<lenOfRootFS*8;i++)
	{
		if (GetFile(&f,i,rootFS))
		{
			for (k=0;k<28;k++)
			{
				WriteToRealMem((DWORD*)(s+k+j*32),(DWORD)f.name[k]);
			}
			j++;
		}
	}
	WriteToRealMem((DWORD*)(s+j*32),(DWORD)0);
	return p;
}




int MLX_API Sys_C_GetProcessThreads(DWORD pid)
{
	int i,n=0;
	DWORD *pos;
	pos=(DWORD*)ADDRESS_PCB_LIST;
	PCB *thisPcb=(PCB*)pos;
	for (i=0;i<31;i++)
	{
		if (thisPcb->pid==pid && thisPcb->status!=TSK_NONE)
		{
			n++;
		}
		pos+=(SIZE_OF_PCB/4);
		thisPcb=(PCB*)pos;
	}
	return n;
}

DWORD MLX_API Sys_C_GetMessage(DWORD *msg)
{
	DWORD *tPCB,numsOfMsg,*mqAddr;
	int i;
	tPCB=GetCurrentPCB();
	mqAddr=(DWORD*)*(tPCB+22);
	numsOfMsg=*(tPCB+23);
	if (!numsOfMsg)
	{
		*msg=0;
		return 1;
	}
	*msg=ReadFromRealMem(mqAddr);
	*(msg+1)=ReadFromRealMem(mqAddr+1);
	if (*msg==0xFFFFFFFF)
	{
		return 0;
	}
	for (i=0;i<numsOfMsg*2;i++)
	{
		WriteToRealMem((DWORD*)i,ReadFromRealMem((DWORD*)i+2));
	}
	numsOfMsg--;
	*(tPCB+23)=numsOfMsg;
	return 1;
}


void MLX_API Sys_C_UnlinkPage(DWORD lMem)
{
	UnlinkPage(lMem);
}

void MLX_API Sys_C_SetPixel(DWORD x,DWORD y,DWORD color)
{
	DrawPixel(x,y,color);
}


void MLX_API Sys_C_SendMessage(DWORD pid,DWORD msg,DWORD param)
{
	int i;
	DWORD *tPCB,*addrMQ;
	tPCB=(DWORD*)ADDRESS_PCB_LIST;
	for (i=0;i<31;i++)
	{
		if (*(tPCB+20)==pid)
		{
			addrMQ=(DWORD*)*(tPCB+22);
			break;
		}
		tPCB+=SIZE_OF_PCB/4;
	}
	if ((DWORD)addrMQ==0)
	{
		return;
	}
	i=*(tPCB+23);
	WriteToRealMem(addrMQ+i*2,msg);
	WriteToRealMem(addrMQ+i*2+1,param);
	i++;
	*(tPCB+23)=i;
}


void MLX_API Sys_C_PrintAChar(char v,DWORD pos,DWORD color,DWORD bg)
{
	DrawChar(v,pos,color,bg);
}




void MLX_API Sys_C_ClearScreen(DWORD sy,DWORD sx,DWORD ey,DWORD ex)
{
	DWORD y;
	y=sy;
	while (sx<=ex)
	{
		while (sy<=ey)
		{
			DrawPixel(sx,sy,0xFFFFFF);
			sy++;
		}
		sy=y;
		sx++;
	}
}

void MLX_API Sys_C_DrawRect(DWORD sy,DWORD sx,DWORD ey,DWORD ex,DWORD color)
{
	DWORD y;
	y=sy;
	while (sx<=ex)
	{
		while (sy<=ey)
		{
			DrawPixel(sx,sy,color);
			sy++;
		}
		sy=y;
		sx++;
	}
}








