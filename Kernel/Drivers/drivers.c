/*----------------Comment-------------------------*/
/*Only keyboard driver and HD drivers.            */
/*------------------------------------------------*/


#include "../os.h"
#include "drivers.h"




void MLX_API Driver_C_KeyBoard()									//KB driver c function
{
	unsigned char r=ReadKBRear();
	unsigned char f=ReadKBFront();
	static unsigned char shiftKey=0;
	char bb[2];
	char *keyBuff=(char*)KEY_BUFF_ADDR;
	unsigned char k;
	k=PortIn(0x60);
	if (k==0x2A || k==0x36)
	{
		shiftKey=1;
		return;
	}
	if (k==0xAA || k==0xB6)
	{
		shiftKey=0;
		return;
	}  
	k=GetAsciiCode(k,shiftKey);
	bb[1]=0;
	bb[0]=k;
	if (k==0)
	{
		return;
	}
	if ((r+1)%256==f)
	{   
		return;
	}
	keyBuff[r]=k;
	r++;
	SetKBRear(r);
}

void MLX_API Driver_HD_C_Interruption(unsigned char mStatus,unsigned char rWSwitch)								//HD driver c function
{
	switch (mStatus)
	{
		case 0x58:
					if (!rWSwitch)
					{
						Driver_HD_ReadSector();
					}
					break;
	}
}
