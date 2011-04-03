/**
 * @file integer_pff.h
 * @brief FatFsモジュール用の整数型定義。
 */

/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef INTEGER_PFF_H
#define INTEGER_PFF_H

/* These types must be 16-bit, 32-bit or larger integer */
#ifndef TOPPERS_ITRON_H
typedef int		INT;
typedef unsigned int	UINT;
#endif

/* These types must be 8-bit integer */
typedef char		CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef short		SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types must be 32-bit integer */
typedef long		LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

#endif
