#ifndef _TASK_LED_H_
#define _TASK_LED_H_

#include "target_test.h"

#define TSKPRI_LED  10

#ifndef TOPPERS_MACRO_ONLY

#define DBLED0 0
#define DBLED1 1
#define DBLED2 2
#define DBLED3 3
#define SWLED0 4
#define SWLED1 5
#define SWLED2 6
#define SWLED3 7

void task_led(intptr_t exinf);

#endif

#endif
