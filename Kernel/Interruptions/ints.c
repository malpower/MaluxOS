#include "../os.h"
#include "ints.h"


void MLX_API Error_GP()
{
    char *msg="GP error detected!";
    int i;
    Sys_C_DrawRect(383,383,583,433,0xDDDDDD);
    Sys_C_DrawRect(380,380,580,430,0x000000);
    for (i=0;i<18;i++)
    {
        DrawChar(msg[i],POS(400+i*8,400),0xFF0000,0x000000);
    }
    while (1);
}
