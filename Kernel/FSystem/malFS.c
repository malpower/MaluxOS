/*----------------------Comment---------------------------*/
/*This file implemented the malFS,a file system designed b*/
/*y me.hehe it's simple,but useful enough on our OS.      */
/*--------------------------------------------------------*/



#include "../os.h"
#include "./malFS.h"

char *fileHandleTable;
DWORD *rootFS;
DWORD *adrsSat,*adrsSatL;
FILE sat;
DWORD lenOfRootFS;

int MLX_API GetFile(FILE*,int,DWORD*);
int MLX_API WriteFile(FILE*,DWORD*,DWORD);
DWORD MLX_API AllocASector();

DWORD MLX_API *LoadFile(FILE *tm)			//load a file acording to tm,it returns a file mem map list.
{
	char *p,*n,*r;
	int i=0,pt=1;
	DWORD *fmap;
	DWORD cSec,nextFCD_Sec,nextFCD_Off;
	n=(char*)MallocAPage();
	p=(char*)MallocAPage();
	fmap=MallocAPage();
	for (i=0;i<1024;i++)
	{
		WriteToRealMem(fmap+i,0);
	}
	r=p;
	WriteToRealMem(fmap,(DWORD)r);
	KRNL_HD_ReadSector((DWORD)tm->secNumOfFCD,(DWORD)tm->secNumOfFCD>>8,(DWORD)tm->secNumOfFCD>>24,(DWORD*)n);
	cSec=ReadFromRealMem((DWORD*)(n+tm->offsetOfFCD));
	nextFCD_Off=tm->offsetOfFCD;
	i=0;
	while (1)
	{
		if (i==8)
		{

			p=(char*)MallocAPage();
			WriteToRealMem(fmap+pt,(DWORD)p);
			pt++;
			i=0;
		}
		KRNL_HD_ReadSector(cSec,cSec>>8,cSec>>24,(DWORD*)p);
		p+=512;
		nextFCD_Sec=ReadFromRealMem((DWORD*)(n+nextFCD_Off+4));
		nextFCD_Off=ReadFromRealMem((DWORD*)(n+nextFCD_Off+8));
		if (!nextFCD_Sec)
		{
			break;
		}
		KRNL_HD_ReadSector((DWORD)nextFCD_Sec,(DWORD)nextFCD_Sec>>8,(DWORD)nextFCD_Sec>>24,(DWORD*)n);
		cSec=ReadFromRealMem((DWORD*)(n+nextFCD_Off));
		i++;
	}
	FreeAPage((DWORD*)n);
	return fmap;
}


DWORD MLX_API *GetFsDirectory(DWORD secNum)			//it returns the pointer for a fs dir.
{
	int i;
	DWORD *m,*r;
	m=MallocAPage();
	r=m;
	while (1)
	{
		for (i=0;i<8;i++)
		{
			KRNL_HD_ReadSector(secNum,secNum>>8,secNum>>24,m);
			WriteToRealMem(m+121,secNum);
			if (ReadFromRealMem(m+120)==0)
			{
				WriteToRealMem(m+122,0);
				return r;
			}
			else
			{
				WriteToRealMem(m+122,(DWORD)m+512);
				secNum=ReadFromRealMem(m+120);
			}
			m+=128;
		}
		m-=128;
		WriteToRealMem(m+122,(DWORD)MallocAPage());
		m=(DWORD*)ReadFromRealMem(m+122);
	}
	return r;
}


int MLX_API GetFreeFD(DWORD *dir)			//it returns a free FD number.
{
	char *p=(char*)dir;
	DWORD *r=(DWORD*)p;
	int i=0;
	while (1)
	{
		if (ReadFromRealMem((DWORD*)(p+3))==0)
		{
			return i;
		}
		if (i%8==0 && i!=0)
		{
			if (ReadFromRealMem(r+120)==0)
			{
				return i;
			}
			p=(char*)ReadFromRealMem(r+122);
			r=(DWORD*)p;
			if (ReadFromRealMem((DWORD*)(p+3))==0)
			{
				return i;
			}
			p+=60;

		}
		else
		{
			p+=60;
		}
		i++;
	}
	return i;
}



DWORD MLX_API AllocASector()				//it returns a new sector number for caller.
{
	int i;
	DWORD nNum=ReadFromRealMem(adrsSat+2);
	DWORD als;
	DWORD *r=MallocAPage(),*n=MallocAPage();
	for (i=0;i<128;i++)
	{
		WriteToRealMem(n+i,0);
	}
	if (nNum==0)
	{
		WriteToRealMem(adrsSat+1,ReadFromRealMem(adrsSat+1)+1);
		KRNL_HD_WriteSector(ReadFromRealMem(adrsSat+1)-1,(ReadFromRealMem(adrsSat+1)-1)>>8,(ReadFromRealMem(adrsSat+1)-1)>>24,n);
		KRNL_HD_WriteSector(190,190>>8,190>>24,adrsSat);
		return ReadFromRealMem(adrsSat+1)-1;
	}
	als=ReadFromRealMem(adrsSat+8);
	KRNL_HD_WriteSector(als,als>>8,als>>24,n);
	WriteToRealMem(adrsSat+8,ReadFromRealMem(adrsSat+8+nNum-1));
	WriteToRealMem(adrsSat+2,nNum-1);
	for (i=0;i<128;i++)
	{
		WriteToRealMem(r+i,0);
	}
	KRNL_HD_WriteSector(als,als>>8,als>>24,r);
	KRNL_HD_WriteSector(190,190>>8,190>>24,adrsSat);
	FreeAPage(r);
	FreeAPage(n);
	return als;
}



void MLX_API InitFS()  				//initialize file system.
{
	int i;
	DWORD *p;
	fileHandleTable=(char*)MallocAPage();
	for (i=0;i<1024;i++)
	{
		WriteToRealMem((DWORD*)(fileHandleTable+i*4),0);
	}
	rootFS=GetFsDirectory(96);
	p=rootFS;
	lenOfRootFS=1;
	while (ReadFromRealMem(p+122))
	{
		lenOfRootFS++;
		p=(DWORD*)ReadFromRealMem(p+122);
	}
	if (!GetFile(&sat,1,rootFS))
	{
		PrintString("Can't find file: SAT.SYS!!",LINE(0));
		while (1);
	}
	adrsSatL=LoadFile(&sat);
	adrsSat=(DWORD*)ReadFromRealMem(adrsSatL);
}

DWORD MLX_API AllocAFCD()					//it returns a new FCD number for caller.
{
	DWORD i=0,sec;
	DWORD *r=MallocAPage();
	KRNL_HD_ReadSector(ReadFromRealMem(adrsSat+3),ReadFromRealMem(adrsSat+3)>>8,ReadFromRealMem(adrsSat+3)>>24,r);
	for (i=0;i<126;i+=3)
	{
		if (!ReadFromRealMem(r+i))
		{
			FreeAPage(r);
			return i*4;
		}
	}
	sec=AllocASector();
	WriteToRealMem(adrsSat+3,sec);
	KRNL_HD_WriteSector(190,190>>8,190>>24,adrsSat);
	FreeAPage(r);
	return 0;
}


int MLX_API GetFile(FILE *tm,int index,DWORD *dir)		//it gets file informations acording to tm.
{
	char *p=(char*)dir;
	DWORD *r=(DWORD*)p;
	int i;
	for (i=1;i<=index;i++)
	{
		if (i%8==0 && i!=0)
		{
			p=(char*)ReadFromRealMem(r+122);
			r=(DWORD*)p;
		}
		else
		{
			p+=60;
		}

	}
	for (i=0;i<28;i++)
	{
		tm->name[i]=(char)ReadFromRealMem((DWORD*)(p+32+i));
		if (i<16)
		{
			tm->password[i]=(char)ReadFromRealMem((DWORD*)(p+8+i));
		}
	}
	tm->properties=(char)ReadFromRealMem((DWORD*)(p+0));
	tm->access=(char)ReadFromRealMem((DWORD*)(p+1));
	tm->ownerID=(char)ReadFromRealMem((DWORD*)(p+2));
	tm->type=(char)ReadFromRealMem((DWORD*)(p+3));
	tm->size=ReadFromRealMem((DWORD*)(p+4));
	tm->offsetOfFCD=ReadFromRealMem((DWORD*)(p+28));
	tm->secNumOfFCD=ReadFromRealMem((DWORD*)(p+24));
	tm->dir=dir;
	if (tm->type==0)
	{
		return 0;
	}
	return 1;
}

int MLX_API OpenFile(FILE *tm,const char *fileName,DWORD *dir)    //Base on GetFile().
{
	int i;
	char *p;
	for (i=0;i<lenOfRootFS*8;i++)
	{
		if (GetFile(tm,i,dir))
		{
			if (!StrCmp(fileName,tm->name))
			{
				return 1;
			}
		}
	}
	p=(char*)tm;
	for (i=0;i<60;i++)
	{
		p[i]=0;
	}
	return 0;
}



void MLX_API UpdateFile(FILE *tm)					//Renew file informations on disk.
{
	char *p=(char*)tm->dir;
	DWORD *r=(DWORD*)p;
	FILE a;
	int i=0;
	while (i!=8 || ReadFromRealMem(r+122)!=0)
	{
		if (i==8)
		{
			i=0;
			r=(DWORD*)ReadFromRealMem(r+122);
		}
		if (GetFile(&a,i,r))
		{
			if (!StrCmp(a.name,tm->name))
			{
				break;
			}
		}
		i++;
	}
	p+=i*60;
	for (i=0;i<28;i++)
	{
		WriteToRealMem((DWORD*)(p+32+i),tm->name[i]);
		if (i<16)
		{
			WriteToRealMem((DWORD*)(p+8+i),tm->password[i]);
		}
	}
	WriteToRealMem((DWORD*)(p+0),tm->properties);
	WriteToRealMem((DWORD*)(p+1),tm->access);
	WriteToRealMem((DWORD*)(p+2),tm->ownerID);
	WriteToRealMem((DWORD*)(p+3),tm->type);
	WriteToRealMem((DWORD*)(p+4),tm->size);
	WriteToRealMem((DWORD*)(p+24),tm->secNumOfFCD);
	WriteToRealMem((DWORD*)(p+28),tm->offsetOfFCD);
	KRNL_HD_WriteSector(ReadFromRealMem(r+121),ReadFromRealMem(r+121)>>8,ReadFromRealMem(r+121)>>24,r);
}

void MLX_API CreateFile(FILE *tm)						//Create a new file.
{
	char *p=(char*)tm->dir;
	DWORD *r=(DWORD*)p;
	DWORD *n,s;
	int index,i,j;
	index=GetFreeFD(tm->dir);
	for (i=1;i<=index;i++)
	{
		if (i%8==0 && i!=0)
		{
			p=(char*)ReadFromRealMem(r+122);
			if (p==0)
			{
				p=(char*)MallocAPage();
				for (j=0;j<512;j++)
				{
					Wbrm(p+j,0);
				}
				WriteToRealMem(r+122,(DWORD)p);
				s=AllocASector();
				WriteToRealMem(r+120,s);
				KRNL_HD_WriteSector(ReadFromRealMem(r+121),ReadFromRealMem(r+121)>>8,ReadFromRealMem(r+121)>>24,r);
				r=(DWORD*)p;
				WriteToRealMem(r+121,s);
				WriteToRealMem(r+122,0);
				WriteToRealMem(r+120,0);
				lenOfRootFS++;
			}
			r=(DWORD*)p;
		}
		else
		{
			p+=60;
		}
	}
	for (i=0;i<28;i++)
	{
		WriteToRealMem((DWORD*)(p+32+i),tm->name[i]);
		if (i<16)
		{
			WriteToRealMem((DWORD*)(p+8+i),tm->password[i]);
		}
	}
	WriteToRealMem((DWORD*)(p+0),tm->properties);
	WriteToRealMem((DWORD*)(p+1),tm->access);
	WriteToRealMem((DWORD*)(p+2),tm->ownerID);
	WriteToRealMem((DWORD*)(p+3),tm->type);
	WriteToRealMem((DWORD*)(p+4),512);
	tm->offsetOfFCD=AllocAFCD();
	tm->secNumOfFCD=ReadFromRealMem(adrsSat+3);
	n=MallocAPage();
	KRNL_HD_ReadSector(tm->secNumOfFCD,tm->secNumOfFCD>>8,tm->secNumOfFCD>>24,n);
	WriteToRealMem(n+(tm->offsetOfFCD/4),AllocASector());
	WriteToRealMem(n+(tm->offsetOfFCD/4+1),0);
	WriteToRealMem(n+(tm->offsetOfFCD/4+2),0);
	WriteToRealMem((DWORD*)(p+24),tm->secNumOfFCD);
	WriteToRealMem((DWORD*)(p+28),tm->offsetOfFCD);
	KRNL_HD_WriteSector(tm->secNumOfFCD,tm->secNumOfFCD>>8,tm->secNumOfFCD>>24,n);
	KRNL_HD_WriteSector(ReadFromRealMem(r+121),ReadFromRealMem(r+121)>>8,ReadFromRealMem(r+121)>>24,r);
	WriteFile(&sat,adrsSatL,sat.size);
	FreeAPage(n);
}


int MLX_API WriteFile(FILE *tm,DWORD *fmap,DWORD size)					//it writes value into file.
{
	DWORD *r;
	DWORD cSec;
	DWORD s=size;
	DWORD offset,secNum,prvOffset,pSec;
	DWORD *buff;
	int i=0,pt=1,j;
	int sMnt=0;
	buff=(DWORD*)ReadFromRealMem(fmap);
	if (size%512)
	{
		sMnt+=1;
	}
	size-=size%512;
	size/=512;
	sMnt+=size;
	r=MallocAPage();
	offset=tm->offsetOfFCD;
	secNum=tm->secNumOfFCD;
	pSec=ReadFromRealMem(adrsSat+3);
	while (sMnt)
	{
		if (i==8)
		{
			buff=(DWORD*)ReadFromRealMem(fmap+pt);
			pt++;
			i=0;
		}
		prvOffset=offset;
		KRNL_HD_ReadSector(secNum,secNum>>8,secNum>>24,r);
		cSec=ReadFromRealMem(r+(offset/4));
		KRNL_HD_WriteSector(cSec,cSec>>8,cSec>>24,buff);
		sMnt--;
		if (!sMnt)
		{
			break;
		}
		buff+=(512/4);
		secNum=ReadFromRealMem(r+(offset/4)+1);
		offset=ReadFromRealMem(r+(offset/4)+2);
		if (secNum==0)
		{
			offset=AllocAFCD();
			if (pSec!=ReadFromRealMem(adrsSat+3))
			{
				WriteToRealMem(r+(prvOffset/4)+1,ReadFromRealMem(adrsSat+3));
				WriteToRealMem(r+(prvOffset/4)+2,0);
				KRNL_HD_WriteSector(pSec,pSec>>8,pSec>>24,r);
				pSec=ReadFromRealMem(adrsSat+3);
				for (j=0;j<128;j++)
				{
					WriteToRealMem(r+j,0);
				}
				secNum=ReadFromRealMem(adrsSat+3);
				WriteToRealMem(r,AllocASector());
				WriteToRealMem(r+1,0);
				WriteToRealMem(r+2,0);
				KRNL_HD_WriteSector(secNum,secNum>>8,secNum>>24,r);
				offset=0;
			}
			else
			{
				secNum=ReadFromRealMem(adrsSat+3);
				WriteToRealMem(r+(prvOffset/4)+1,secNum);
				WriteToRealMem(r+(prvOffset/4)+2,offset);
				WriteToRealMem(r+(offset/4),AllocASector());
				WriteToRealMem(r+(offset/4)+1,0);
				WriteToRealMem(r+(offset/4)+2,0);
				KRNL_HD_WriteSector(secNum,secNum>>8,secNum>>24,r);
			}
			pSec=ReadFromRealMem(adrsSat+3);
			WriteFile(&sat,adrsSatL,sat.size);
		}
		i++;
	}
	if (s>tm->size)
	{
		tm->size=s;
		UpdateFile(tm);
	}
	FreeAPage(r);
	return 1;
}


void MLX_API RegisterUnusedSector(DWORD sec)				//free a sector.
{
	int i;
	for (i=8;i<(sat.size/4);i++)
	{
		if (!ReadFromRealMem(adrsSat+i))
		{
			WriteToRealMem(adrsSat+i,sec);
		}
	}
	WriteToRealMem(adrsSat+2,ReadFromRealMem(adrsSat+2)+1);
	WriteFile(&sat,adrsSatL,sat.size);
}



void MLX_API DeleteFile(FILE *tm)					//delete file.
{
	char *p=(char*)tm->dir;
	DWORD *r=(DWORD*)p;
	FILE a;
	DWORD secNum,offset,cSec,cOff;
	int i=0;
	while (1)
	{
		if (i%8==0 && i!=0)
		{
			r=(DWORD*)ReadFromRealMem(r+122);
			if (r==0)
			{
				return;
			}
			p=(char*)r;
			i=0;
		}
		if (GetFile(&a,i,r))
		{
			if (!StrCmp(tm->name,a.name))
			{
				break;
			}
		}
		i++;
		p+=60;
	}
	for (i=0;i<28;i++)
	{
		WriteToRealMem((DWORD*)(p+32+i),0);
		if (i<16)
		{
			WriteToRealMem((DWORD*)(p+8+i),0);
		}
	}
	WriteToRealMem((DWORD*)(p+0),0);
	WriteToRealMem((DWORD*)(p+1),0);
	WriteToRealMem((DWORD*)(p+2),0);
	WriteToRealMem((DWORD*)(p+3),0);
	WriteToRealMem((DWORD*)(p+4),0);
	WriteToRealMem((DWORD*)(p+24),0);
	WriteToRealMem((DWORD*)(p+28),0);
	KRNL_HD_WriteSector(ReadFromRealMem(r+121),ReadFromRealMem(r+121)>>8,ReadFromRealMem(r+121)>>24,r);
	r=MallocAPage();
	offset=tm->offsetOfFCD;
	secNum=tm->secNumOfFCD;
	while (secNum)
	{
		KRNL_HD_ReadSector(secNum,secNum>>8,secNum>>24,r);

		WriteToRealMem(r+(offset/4),0);
		cSec=ReadFromRealMem(r+(offset/4)+1);
		cOff=ReadFromRealMem(r+(offset/4)+2);
		WriteToRealMem(r+(offset/4)+1,0);
		WriteToRealMem(r+(offset/4)+2,0);
		KRNL_HD_WriteSector(secNum,secNum>>8,secNum>>24,r);
		secNum=cSec;
		offset=cOff;
	}
	FreeAPage(r);
	return;
}


