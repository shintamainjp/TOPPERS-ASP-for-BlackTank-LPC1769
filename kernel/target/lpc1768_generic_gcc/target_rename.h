/* This file is generated from target_rename.def by genrename. */

#ifndef TOPPERS_TARGET_RENAME_H
#define TOPPERS_TARGET_RENAME_H


#define target_initialize			_kernel_target_initialize
#define target_exit				_kernel_target_exit


#ifdef TOPPERS_LABEL_ASM

/*
 * target_config.c
 */
#define _target_initialize			__kernel_target_initialize
#define _target_exit				__kernel_target_exit

 /*
  *  trace_config.c
  */
#define log_dsp_enter _kernel_log_dsp_enter
#define log_dsp_leave _kernel_log_dsp_leave
#define log_inh_enter _kernel_log_inh_enter
#define log_inh_leave _kernel_log_inh_leave
#define log_exc_enter _kernel_log_exc_enter
#define log_exc_leave _kernel_log_exc_leave


#endif /* TOPPERS_LABEL_ASM */

#include "chip_rename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
