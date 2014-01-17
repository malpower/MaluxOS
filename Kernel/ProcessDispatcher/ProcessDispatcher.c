#include "../os.h"
#include "ProcessDispatcher.h"
#include "../support.h"


DWORD *cPcbAdrs;

void MLX_API ProcessDispatcher()
{
	char *pcbSwap=(char*)ADDRESS_PCB_SWAP;
	char *sPcb=(char*)ADDRESS_PCB_LIST;
	PCB *tPcb;
	DWORD *tmp;
	int i,j;
	DWORD *c3;
    tPcb=(PCB*)pcbSwap;
	if (tPcb->cs==CS_SYSTEM)							//do NOT schedule if this is KERNEL running time.
	{
		return;
	}
	for (i=0;i<=LEN_OF_PCB_LIST;i++)												//a simple schedule algorithm
	{
		tPcb=(PCB*)sPcb;
		if (tPcb->status==TSK_RUNNING || tPcb->status==TSK_DESTROYING)
		{
			for (j=0;j<SIZE_OF_PCB_SWAP;j++)
			{
				sPcb[j]=pcbSwap[j];
			}
			tPcb=(PCB*)sPcb;
			if (tPcb->status==TSK_DESTROYING)
			{
				tPcb->status=TSK_NONE;
                tPcb->pid=0;
			}
			else
			{
				tPcb->status=TSK_READY;
			}
			sPcb=(char*)ADDRESS_PCB_LIST+(tPcb->nextp*SIZE_OF_PCB);                                          //PCB structure is in another document.
			tPcb=(PCB*)sPcb;
			while (tPcb->status==TSK_WATING)
			{
				sPcb=(char*)ADDRESS_PCB_LIST+(tPcb->nextp*SIZE_OF_PCB);
				tPcb=(PCB*)sPcb;
			}
			tPcb->status=TSK_RUNNING;
			for (j=0;j<SIZE_OF_PCB_SWAP;j++)
			{
				pcbSwap[j]=sPcb[j];
			}
			c3=(DWORD*)(sPcb+84);
			tmp=(DWORD*)(sPcb+64);
			LoadCr3(*c3);											//                                          Load Cr3 for next process.
			ReloadLDT(*tmp);										//                                          Load LDTR for next process.
			cPcbAdrs=(DWORD*)sPcb;
			return;
		}
		sPcb+=SIZE_OF_PCB;
	}
}
