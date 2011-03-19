#ifndef _TASK_MENU_H_
#define _TASK_MENU_H_

#include "target_test.h"

#define TSKPRI_MENU    11

#ifndef TOPPERS_MACRO_ONLY

#define SW0 0
#define SW1 1
#define SW2 2
#define SW3 3
#define VOL0 4
#define VOL1 5
#define VOL2 6
#define VOL3 7

#define USERMSG(device, value) snd_dtq(DTQ_USERINPUT, (intptr_t)(((device) << 5) | ((value) & 0x1F)))

void task_menu(intptr_t exinf);

#endif

#endif

