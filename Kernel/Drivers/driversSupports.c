#include "../os.h"
#include "driversSupports.h"

char MLX_API GetAsciiCode(unsigned char scanCode,unsigned char isShift)					//translate scancode into ascii code.
{
	static char trsTable[]={0x9E,'a',
					 		0xB0,'b',
					 		0xAE,'c',
					 		0xA0,'d',
					 		0x92,'e',
					 		0xA1,'f',
					 		0xA2,'g',
					 		0xA3,'h',
					 		0x97,'i',
					 		0xA4,'j',
							0xA5,'k',
							0xA6,'l',
							0xB2,'m',
							0xB1,'n',
							0x98,'o',
							0x99,'p',
							0x90,'q',
							0x93,'r',
							0x9F,'s',
							0x94,'t',
							0x96,'u',
							0xAF,'v',
							0x91,'w',
							0xAD,'x',
							0x95,'y',
							0xAC,'z',
							0x8B,'0',
							0x82,'1',
							0x83,'2',
							0x84,'3',
							0x85,'4',
							0x86,'5',
							0x87,'6',
							0x88,'7',
							0x89,'8',
							0x8A,'9',
							0x81,'`',
							0x8C,'-',
							0x8D,'=',
							0xAB,'\\',
							0x8E,8,
							0x9C,13,
							0xB9,' ',
							0xA7,';',
							0xA8,'\'',
							0x9B,']',
							0xB3,',',
							0xB5,'/',
							0x9A,'[',
							0xB4,'.',
							0xD2,'0',
							0xCF,'1',
							0xD0,'2',
							0xD1,'3',
							0xCB,'4',
							0xCC,'5',
							0xCD,'6',
							0xC7,'7',
							0xC8,'8',
							0xC9,'9'};
							
	int i=0;
	char sc=scanCode;
	for (i=0;i<60;i++)
	{	
		if (trsTable[i*2]==sc)
		{
			if (isShift && i<26)
			{
				return trsTable[i*2+1]-32;
			}
			else
			{
				if (isShift)
				{
					switch (trsTable[i*2])
					{
						case (char)0x8B:
									return ')';
						case (char)0x82: 
									return '!';
						case (char)0x83:
									return '@';
						case (char)0x84:
									return '#';	
						case (char)0x85:
									return '$';
						case (char)0x86:
									return '%';
						case (char)0x87:
									return '^';
						case (char)0x88:
									return '&';
						case (char)0x89:
									return '*';
						case (char)0x8A:
									return '(';
						case (char)0x81:
									return '~';
						case (char)0x8C:
									return '_';
						case (char)0x8D:
									return '+';
						case (char)0xAB:
									return '|';
						case (char)0x9C:
									return 13;
						case (char)0xB5:
									return '?';
						case (char)0xB4:
									return '>';
						case (char)0xB3:
									return '<';
						case (char)0xA8:
									return '\"';
						case (char)0xA7:
									return ':';
						case (char)0x9B:
									return '}';
						case (char)0x9A:
									return '{';
					}
				}
				return trsTable[i*2+1];		
			}
		}

	}
	return 0;
}





