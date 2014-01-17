#define MLX_API __attribute__((cdecl))
#define LINE(x) (x<<16)
#define POS(y,x) ((x<<16)+y)
typedef unsigned long DWORD;

typedef struct
{
	DWORD x;
	DWORD y;
	DWORD width;
	DWORD height;
	DWORD bgColor;
	char *title;
	DWORD check;
	DWORD tColor;
}WND;


typedef struct
{
	DWORD msg;
	DWORD param;
}MSG;

extern void MLX_API MLX_PrintChar(char,DWORD,DWORD,DWORD);
extern void MLX_API MLX_CreateThread_R(void(*),DWORD*);
extern void MLX_API *MLX_MAlloc();


void MLX_API MLX_CreateThread(void(*fn))
{
	DWORD *x=(DWORD*)MLX_MAlloc();
	x+=1000;
	MLX_CreateThread_R(fn,x);
}


void MLX_API MLX_PrintString(char *c,DWORD pos,DWORD color,DWORD bg)
{
	int i=0;
	while (c[i]!=0)
	{
		MLX_PrintChar(c[i],pos+i*9,color,bg);
		i++;
	}
}


void MLX_API MLX_PrintNumber(DWORD key,DWORD pos,DWORD color,DWORD bg)
{
	int i,t;
	char k[10],c;
	for (i=0;i<9;i++)
	{
		t=key%10;
		key-=t;
		key/=10;
		t+=48;
		k[i]=(char)t;
	}
	for (i=8;i>0;i--)
	{
		if (k[i]=='0')
		{
			k[i]=0;
		}
		else
		{
			break;
		}
	}
	t=0;
	while (t<i)
	{
		c=k[t];
		k[t]=k[i];
		k[i]=c;
		t++;
		i--;
	}
	k[9]=0;
	MLX_PrintString(k,pos,color,bg);
}



DWORD MLX_API MLX_ToDword(char *s)
{
	DWORD rslt;
	int i,len,p;
	char t;
	len=0;
	while (s[len])
	{
		len++;
	}
	len--;
	p=len;
	for (i=0;i<p;i++,p--)
	{
		t=s[i];
		s[i]=s[p];
		s[p]=t;
	}
	rslt=0;
	p=1;
	for (i=0;i<len+1;i++)
	{
		if (s[i]>47 && s[i]<59)
		{
			s[i]-=48;
			rslt+=s[i]*p;
			p*=10;
		}
		else
		{
			break;
		}
	}
	return rslt;
}



void MLX_API MLX_GetInput(char *s,DWORD pos,int size,DWORD color,DWORD bg)
{
	char k;
	int i=0;
	k=MLX_ReadKeyBuff();
	MLX_PrintChar('_',pos,color,bg);
	while (k!=13)
	{
		if (k && i<size)
		{
			if (k==8)
			{
				if (i>0)
				{
					i--;
					MLX_PrintString("_ ",pos+i*9,color,bg);
				}
			}
			else
			{
				s[i]=k;
				MLX_PrintChar(k,pos+i*9,color,bg);
				i++;
				MLX_PrintChar('_',pos+i*9,color,bg);
			}
		}
		else
		{
			MLX_DiscardCPU();
		}
		k=MLX_ReadKeyBuff();
	}
	s[i]=0;
}


void MLX_API MLX_Pause()
{
	while (!MLX_ReadKeyBuff())
	{
		MLX_DiscardCPU();
	}
}

void MLX_API MLX_PrintNumberAsHex(DWORD num,long pos,DWORD color,DWORD bg)
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
	MLX_PrintString((char*)p,pos,color,bg);
}


int MLX_API MLX_StrCmp(const char *a,const char *b)
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

void MLX_API MLX_CreateWindow(WND *wnd,DWORD x,DWORD y,DWORD width,DWORD height,DWORD bg,char *title)
{
	if (x+width>=1024 || y+height>=768)
	{
		wnd->check=0;
		return;
	}
	wnd->x=x;
	wnd->y=y;
	wnd->width=width;
	wnd->height=height;
	wnd->bgColor=bg;
	wnd->title=title;
	wnd->check=1;
	wnd->tColor=0x2288DD;
}

void MLX_API MLX_ShowWindow(WND *wnd)
{
	if (wnd && wnd->check)
	{
		MLX_DrawRect(wnd->x+3,wnd->y+3,wnd->x+wnd->width+3,wnd->y+wnd->height+3,0xDDDDDD);
		MLX_DrawRect(wnd->x,wnd->y,wnd->x+wnd->width,wnd->y+wnd->height,0x0);
		MLX_DrawRect(wnd->x+1,wnd->y+1,wnd->x+wnd->width-1,wnd->y+wnd->height-1,wnd->bgColor);
		MLX_DrawRect(wnd->x+1,wnd->y+1,wnd->x+wnd->width-1,wnd->y+20,wnd->tColor);
		MLX_DrawRect(wnd->x+1,wnd->y+20,wnd->x+wnd->width-1,wnd->y+20,0x0);
		MLX_DrawRect(wnd->x+wnd->width-21,wnd->y+3,wnd->x+wnd->width-6,wnd->y+14,0x0);
		MLX_PrintChar('X',POS(wnd->x+wnd->width-16,wnd->y+4),0xFFFFFF,0);
		MLX_PrintString(wnd->title,POS(wnd->x+20,wnd->y+5),0xFFFFFF,wnd->tColor);
	}
}

void MLX_API MLX_DarkWindow(WND *wnd)
{
	if (wnd && wnd->check)
	{
		MLX_DrawRect(wnd->x+1,wnd->y+1,wnd->x+wnd->width-1,wnd->y+19,0xEFEFEF);
		MLX_DrawRect(wnd->x+wnd->width-21,wnd->y+3,wnd->x+wnd->width-6,wnd->y+14,0x0);
		MLX_PrintChar('X',POS(wnd->x+wnd->width-16,wnd->y+4),0xFFFFFF,0);
		MLX_PrintString(wnd->title,POS(wnd->x+20,wnd->y+5),0xFFFFFF,0xEFEFEF);
		wnd->tColor=0xEFEFEF;
	}
}

void MLX_API MLX_LightWindow(WND *wnd)
{
	if (wnd && wnd->check)
	{
		MLX_DrawRect(wnd->x+1,wnd->y+1,wnd->x+wnd->width-1,wnd->y+19,0x2288DD);
		MLX_DrawRect(wnd->x+wnd->width-21,wnd->y+3,wnd->x+wnd->width-6,wnd->y+14,0x0);
		MLX_PrintChar('X',POS(wnd->x+wnd->width-16,wnd->y+4),0xFFFFFF,0);
		MLX_PrintString(wnd->title,POS(wnd->x+20,wnd->y+5),0xFFFFFF,0x2288DD);
		wnd->tColor=0x2288DD;
	}
}

void MLX_API MLX_HideWindow(WND *wnd)
{
	if (wnd && wnd->check)
	{
		MLX_DrawRect(wnd->x,wnd->y,wnd->x+wnd->width+3,wnd->y+wnd->height+3,0xFFFFFF);
	}
}

void MLX_API MLX_TextOut(WND *wnd,DWORD x,DWORD y,char *v,DWORD color)
{
	if (wnd && wnd->check)
	{
		MLX_PrintString(v,POS(x+wnd->x+1,y+wnd->y+21),color,wnd->bgColor);
	}
}

void MLX_API MLX_NumberOut(WND *wnd,DWORD x,DWORD y,DWORD key,DWORD color)
{
	int i,t;
	char k[10],c;
	for (i=0;i<9;i++)
	{
		t=key%10;
		key-=t;
		key/=10;
		t+=48;
		k[i]=(char)t;
	}
	for (i=8;i>0;i--)
	{
		if (k[i]=='0')
		{
			k[i]=0;
		}
		else
		{
			break;
		}
	}
	t=0;
	while (t<i)
	{
		c=k[t];
		k[t]=k[i];
		k[i]=c;
		t++;
		i--;
	}
	k[9]=0;
	MLX_TextOut(wnd,x,y,k,color);
}

DWORD MLX_API MLX_GetMessage(MSG *appMsg)
{
	DWORD k;
	while (1)
	{
		k=MLX_GetMessageASM(appMsg);
		if (appMsg->msg==0)
		{
			MLX_DiscardCPU();
			continue;
		}
		else
		{
			break;
		}
	}
	return k;
}






