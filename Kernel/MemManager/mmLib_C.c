#include "../os.h"
#include "./mmLib.h"

void MLX_API ClearMap()
{
	char *p=0;
	DWORD i;
	DWORD mSize=GetMemSize();
	for (i=0;i<1024*1024;i++)
	{
		WriteMMap(p,0xFF);
		p++;
	}
	mSize+=1024;
	mSize/=4;
	p=0;
	for (i=0;i<mSize;i++)
	{
		WriteMMap(p,0);
		p++;
	}
	p=0;
	for (i=0;i<2560;i++)
	{
		WriteMMap(p,0xFF);
		p++;
	}
}

DWORD MLX_API *MallocAPage()
{
	DWORD i;
	char *p=0;
	DWORD address=0;
	for (i=0;i<1024*1024;i++)
	{
		if (ReadMMap(p)==0)
		{
			WriteMMap(p,0xFF);
			return (DWORD*)address;
		}
		address+=4096;
		p++;
	}
	return (DWORD*)0;
}

void MLX_API FreeAPage(DWORD *address)
{
	char *p=0;
	unsigned int t=(unsigned int)address%(SIZE_OF_PAGE/4);
	address-=(t/4);
	if (address==0)
	{
		return;
	}
	while (address!=0)
	{
		p++;
		address-=(SIZE_OF_PAGE/4);
	}
	WriteMMap(p,0);
}


DWORD MLX_API *LinkPage(DWORD pid,DWORD *pMem,DWORD aMode)
{
	DWORD *PDE,*s,tmp;
    PCB *cPcb;
	char *p;
	DWORD addr=0;
	int i,j;
	cPcb=(PCB*)GetPCB(pid);
	PDE=(DWORD*)cPcb->cr3;
	PDE++;
	s=(DWORD*)ReadFromRealMem(PDE);
	p=(char*)s;
	tmp=(DWORD)p;
	tmp>>=12;
	tmp<<=12;
	p=(char*)tmp;
	s=(DWORD*)p;
	for (i=0;i<1023;i++)
	{
		for (j=0;j<1024;j++)
		{
			if (ReadFromRealMem(s)==0)
			{
				p=(char*)pMem;
				p+=aMode;
				WriteToRealMem(s,(DWORD)p);
				return (DWORD*)addr;
			}
			s++;
			addr+=SIZE_OF_PAGE;
		}
		PDE++;
		if (ReadFromRealMem(PDE)==0)
		{
			break;
		}
		s=(DWORD*)ReadFromRealMem(PDE);
		p=(char*)s;
		tmp=(DWORD)p;
		tmp>>=12;
		tmp<<=12;
		p=(char*)tmp;
		s=(DWORD*)p;
	}
	PDE=(DWORD*)cPcb->cr3;
	for (i=0;i<1024;i++)
	{
		if (ReadFromRealMem(PDE)==0)
		{
			p=(char*)MallocAPage();
			p+=aMode;
			WriteToRealMem(PDE,(DWORD)p);
			p-=aMode;
			PDE=(DWORD*)p;
			p=(char*)pMem;
			p+=aMode;
			WriteToRealMem(PDE,(DWORD)p);
			break;
		}
		PDE++;
		addr+=0x00400000;
	}
	addr-=0x00400000;
	return (DWORD*)addr;
}


void MLX_API UnlinkPage(DWORD lMem)
{
	DWORD *p;
	DWORD i;
	lMem+=0x00400000;
	p=GetCurrentPCB();
	p+=21;
	*p>>=12;
	*p<<=12;
	p=(DWORD*)*p;
	while (lMem>=0x00400000)
	{
		p++;
		lMem-=0x00400000;
	}
	lMem/=0x00001000;
	i=ReadFromRealMem(p);
	i>>=12;
	i<<=12;
	p=(DWORD*)i;
	for (i=0;i<lMem;i++)
	{
		p++;
	}
	i=ReadFromRealMem(p);
	i>>=12;
	i<<=12;
	WriteToRealMem(p,0);
	FreeAPage((DWORD*)i);
}


DWORD MLX_API *GetPAddr(DWORD lMem)
{
	DWORD *p;
	DWORD i;
	lMem+=0x00400000;
	p=GetCurrentPCB();
	p+=21;
	*p>>=12;
	*p<<=12;
	p=(DWORD*)*p;
	while (lMem>=0x00400000)
	{
		p++;
		lMem-=0x00400000;
	}
	lMem/=0x00001000;
	i=ReadFromRealMem(p);
	i>>=12;
	i<<=12;
	p=(DWORD*)i;
	for (i=0;i<lMem;i++)
	{
		p++;
	}
	i=ReadFromRealMem(p);
	i>>=12;
	i<<=12;
	return (DWORD*)i;
}







