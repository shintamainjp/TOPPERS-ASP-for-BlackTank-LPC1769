/* This file is generated from target_rename.def by genrename. */

/* This file is included only when target_rename.h has been included. */
#ifdef TOPPERS_TARGET_RENAME_H
#undef TOPPERS_TARGET_RENAME_H


#undef target_initialize
#undef target_exit


#ifdef TOPPERS_LABEL_ASM

/*
 * target_config.c
 */
#undef _target_initialize
#undef _target_exit

/*
 *  trace_config.c
 */
#undef log_dsp_enter
#undef log_dsp_leave
#undef log_inh_enter
#undef log_inh_leave
#undef log_exc_enter
#undef log_exc_leave


#endif /* TOPPERS_LABEL_ASM */

#include "chip_unrename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
