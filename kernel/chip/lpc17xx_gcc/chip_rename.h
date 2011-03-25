/* This file is generated from chip_rename.def by genrename. */

#ifndef TOPPERS_CHIP_RENAME_H
#define TOPPERS_CHIP_RENAME_H


#define chip_initialize				_kernel_chip_initialize
#define chip_exit				_kernel_chip_exit
#define chip_fput_log				_kernel_chip_fput_log

#ifdef TOPPERS_LABEL_ASM


#define _chip_initialize			__kernel_chip_initialize
#define _chip_exit				__kernel_chip_exit
#define _chip_fput_log				__kernel_chip_fput_log

#endif /* TOPPERS_LABEL_ASM */

#include "arm_m_gcc/prc_rename.h"

#endif /* TOPPERS_CHIP_RENAME_H */
