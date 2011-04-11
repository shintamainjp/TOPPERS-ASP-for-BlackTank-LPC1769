#!/bin/sh

#
# gdb経由でプログラムをロードする。
#

arm-none-eabi-gdb TOPPERS-ASP_BlackTank-LPC1769.axf -x gdbcmd-progload.txt

