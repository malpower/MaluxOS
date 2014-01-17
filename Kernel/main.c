/*---------------------Comment------------------*/
/*  This file is the start of malux's running.  */
/* it will install the OS and jump into CLI.    */
/*  first,it call fucntion GetMemSize() for gett*/
/*ing the size of RAM.Function InitMem() is used*/
/*to initialize mem managers and mem evironment.*/
/*InitInt() is used to initialize interruptions.*/
/*Installer() installs the syscalls and some oth*/
/*er things.GetInUI() makes a process named Temp*/
/*UI,which will use the StartEUI syscall to star*/
/*t BUI.SYS on disk.                            */
/*----------------------------------------------*/



#include "os.h"
#include "main.h"

int main()										//main function ,from address 0 on.
{
	InitMem();					//call memory initializing function.
	InitInt();						//call interruption system initializing fucntion.
	InitTSS();											//call TSS initializing function.
	Installer();						 					//call system installer for block installings.
	GetInUI();											//after all initializings,jump into UI.
	SystemStop();
	return 0;
}
