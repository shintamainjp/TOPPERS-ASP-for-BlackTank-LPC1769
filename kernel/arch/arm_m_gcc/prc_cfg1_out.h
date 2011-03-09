/*
 *  @(#) $Id: prc_cfg1_out.h 1304 2008-08-27 07:28:36Z ertl-honda $
 */

/*
 *		cfg1_out.cをリンクするために必要なスタブの定義
 */

void sta_ker(void){}
#ifdef TOPPERS_CHIP_LPC17XX
/* TOPPERS_CHIP_LPC17XXが宣言されているときには、何もここでは宣言しない */
#else
STK_T *const	_kernel_istkpt = 0x00;
#endif
