/*---------------------Comment------------------*/
/*This file declared many useful functions.     */
/*----------------------------------------------*/






#include "os.h"
#include "support.h"

static unsigned char foq=0;             					//a control various for KB driver
static unsigned char roq=1;									//another control various for KB dirver


extern DWORD newPID;
DWORD *cLib;


void MLX_API DrawPixel(DWORD,DWORD,DWORD);


void MLX_API DrawChar(char v,DWORD pos,DWORD color,DWORD bg)
{
	DWORD cL,cH;
	int i,j;
	DWORD x,y;
	unsigned char m;
	char *p=(char*)cLib;
	y=(short)pos;
	x=pos>>16;
	p+=(8*v);
	cL=ReadFromRealMem((DWORD*)p);
	cH=ReadFromRealMem((DWORD*)(p+4));
	for (i=0;i<4;i++)
	{
		m=(unsigned char)cL;
		for (j=0;j<8;j++)
		{
			if ((unsigned char)m&0x80)
			{
				DrawPixel(x,y,color);
			}
			else
			{
				DrawPixel(x,y,bg);
			}
			y++;
			m<<=1;
		}
		x++;
		y=(short)pos;
		cL>>=8;
		m=(unsigned char)cL;
	}
	for (i=0;i<4;i++)
	{
		m=(unsigned char)cH;
		for (j=0;j<8;j++)
		{
			if ((unsigned char)m&0x80)
			{
				DrawPixel(x,y,color);
			}
			else
			{
				DrawPixel(x,y,bg);
			}
			y++;
			m<<=1;
		}
		x++;
		cH>>=8;
		y=(short)pos;
		m=(unsigned char)cH;
	}
}

void MLX_API SystemStop()					//Error processing function,just simple while(1)
{
	while (1);
}

void MLX_API PrintString(char *str,long pos)
{
	int i=0;
	while (str[i]!=0)
	{
		PrintA(str[i],(long)(pos+i));
		i++;
	}
}

void MLX_API DrawString(char *str,long pos)
{
	int i=0;
	while (str[i]!=0)
	{
		DrawChar(str[i],(DWORD)(pos+i*8),0,0xFFFFFF);
		i++;
	}
}

void MLX_API PrintNumberAsHex(DWORD num,long pos)    //Print a number onto screen as hex.
{
	unsigned char p[11]={0};
	int i;
	for (i=9;i>=2;i--)
	{
		p[i]=(char)num;
		p[i]<<=4;
		p[i]>>=4;
		if (p[i]>9)
		{
			p[i]+=55;
		}
		else
		{
			p[i]+=48;
		}
		num>>=4;
	}
	p[0]='0';
	p[1]='x';
	PrintString((char*)p,pos);
}




void MLX_API DrawNumberAsHex(DWORD num,long pos)    //Print a number onto screen as hex.
{
	unsigned char p[11]={0};
	int i;
	for (i=9;i>=2;i--)
	{
		p[i]=(char)num;
		p[i]<<=4;
		p[i]>>=4;
		if (p[i]>9)
		{
			p[i]+=55;
		}
		else
		{
			p[i]+=48;
		}
		num>>=4;
	}
	p[0]='0';
	p[1]='x';
	DrawString((char*)p,pos);
}
unsigned char MLX_API ReadKBFront()					//read KB buff front pointer.
{
	return foq;
}

unsigned char MLX_API ReadKBRear()					//read KB buff rear pointer.
{
	return roq;
}

void MLX_API SetKBFront(unsigned char f)					//set KB buff front pointer.
{
	foq=f;
}

void MLX_API SetKBRear(unsigned char r)						//set Kb buff rear pointer.
{
	roq=r;
}


char MLX_API *GetFreePCB()							//Search for a free PCB element.
{
	int i;
	char *pos;
	pos=(char*)ADDRESS_PCB_LIST;
	pos+=(17*4);
	for (i=0;i<31;i++)
	{
		if ((*pos)==0)
		{
			pos-=(17*4);
			return pos;
		}
		pos+=SIZE_OF_PCB;
	}
	return 0;
}

char MLX_API *GetCurrentPCB()							//Search for a free PCB element.
{
	int i;
	char *pos;
	pos=(char*)ADDRESS_PCB_LIST;
	pos+=(17*4);
	for (i=0;i<31;i++)
	{
		if ((*pos)==2)
		{
			pos-=(17*4);
			return pos;
		}
		pos+=SIZE_OF_PCB;
	}
	return 0;
}


char MLX_API *GetPCB(DWORD pid)							//Search for a free PCB element.
{
	int i;
	DWORD *pos;
	pos=(DWORD*)ADDRESS_PCB_LIST;
	pos+=20;
	for (i=0;i<31;i++)
	{
		if (*pos==pid && *(pos-3)!=0)
		{
			pos-=20;
			return (char*)pos;
		}
		pos+=SIZE_OF_PCB/4;
	}
	return 0;
}

DWORD MLX_API *CreateCr3(DWORD *pm)							//calculate a cr3 address for new process
{
	DWORD *m,*mi,*pde,*pdei;
	DWORD k=0;
	int i;
	m=MallocAPage();
	mi=m;
	WriteToRealMem(m,((DWORD)pm)+7);
	m++;
	for (i=1;i<1024;i++)
	{
		WriteToRealMem(m,k);
		m++;
	}
	pde=MallocAPage();
	pdei=pde;
	for (i=0;i<1023;i++)
	{
		WriteToRealMem(pde,0);
		pde++;
	}
	WriteToRealMem(pdei+1,((DWORD)mi)+7);
	WriteToRealMem(pdei,(DWORD)0x00201007);
	WriteToRealMem(pde,0x00201407);
	return pdei;
}

cTime MLX_API *KRNL_GetTime()
{
	return (cTime*)0;
}

void MLX_API StrCpy(char *a,const char *b)				//String copying.
{
	int i=0;
	while (b[i])
	{
		a[i]=b[i];
		i++;
	}
	a[i]=0;
}

DWORD MLX_API GetFreeLDT()
{
	static DWORD list[30];
	int i;
	for (i=0;i<30;i++)
	{
		if (list[i]==0)
		{
			list[i]=1;
			return (i+1)*0x100;
		}
	}
	return 0;
}


void MLX_API DrawPixel(DWORD y,DWORD x,DWORD color)
{
	extern DWORD *gRam;
	char *p=(char*)gRam;
	if (y>=768 || x>=1024)
	{
		return;
	}
	y*=1024;
	Wbrm(p+((x+y)*3),(char)color);
	color>>=8;
	Wbrm(p+((x+y)*3+1),(char)color);
	color>>=8;
	Wbrm(p+((x+y)*3+2),(char)color);

}


void MLX_API StartEUI()										//a function for creating a new process which is BUI.SYS
{
	extern DWORD *rootFS;
	DWORD *address;
	DWORD esp;
	int i=1,j;
	FILE bui;
	if (OpenFile(&bui,"BUI.SYS",rootFS))							//Load Basic User Interface.filename is "BUI.SYS".
	{
		address=LoadFile(&bui);
		CreateProcess(GetFreeLDT(),(DWORD*)ReadFromRealMem(address),0x1FF0);
		while (ReadFromRealMem(address+i))
		{
			LinkPage(newPID-1,(DWORD*)ReadFromRealMem(address+i),PG_RW);
			i++;
		}
		FreeAPage(address);
		address=MallocAPage();
		esp=(DWORD)LinkPage(newPID-1,address,PG_RW);
		esp+=4080;
		address=(DWORD*)GetPCB(newPID-1);
		*(address+3)=esp;
	}
	for (i=0;i<768;i++)
	{
		for (j=0;j<1024;j++)
		{
			DrawPixel(i,j,0xFFFFFF);
		}
	}
	cLib=MallocAPage();
	KRNL_HD_ReadSector(70,0,0,cLib);
	KRNL_HD_ReadSector(71,0,0,cLib+512/4);
}

void MLX_API FreePageTree(DWORD *cr3)				//Free a mem page tree acording to cr3
{
	DWORD *p;
	DWORD s;
	int i,j;
	for (i=1;i<1023;i++)
	{
		if (ReadFromRealMem(cr3+i))
		{
			s=ReadFromRealMem(cr3+i);
			s&=0xFFFFF000;
			p=(DWORD*)s;
			for (j=0;j<1024;j++)
			{
				if (ReadFromRealMem(p+j))
				{
					s=ReadFromRealMem(p+j);
					s&=0xFFFFF000;
					FreeAPage((DWORD*)s);
				}
			}
		}
	}
	for (i=1;i<1023;i++)
	{
		if (ReadFromRealMem(cr3+i))
		{
			s=ReadFromRealMem(cr3+i);
			s&=0xFFFFF000;
			FreeAPage((DWORD*)s);
		}
	}
	s=(DWORD)cr3;
	s&=0xFFFFF000;
	FreeAPage((DWORD*)s);
}

void MLX_API ExitAllThreads(DWORD pid)				//Exit all threads of a process
{
	char *p=(char*)ADDRESS_PCB_LIST;
	DWORD *n;
	char *x=(char*)ADDRESS_PCB_LIST;
	int i;
	for (i=0;i<31;i++)
	{
		n=(DWORD*)p;
		if (*(n+20)==pid)
		{
			*(n+20)=0;
			while (*(x+76)!=((DWORD)p-ADDRESS_PCB_LIST)/0x100)
			{
				x=(char*)(*(x+76)*0x100)+ADDRESS_PCB_LIST;
			}
			*(x+76)=*(p+76);
			*(p+68)=TSK_DESTROYING;
		}
        p+=SIZE_OF_PCB;
	}
}



int MLX_API StrCmp(const char *a,const char *b)		//String comparing.
{
	int i=0;
	while (a[i]!=0)
	{
		if (a[i]!=b[i])
		{
			return i+1;
		}
		i++;
	}
	if (b[i]!=0)
	{
		return i+1;
	}
	return 0;
}


