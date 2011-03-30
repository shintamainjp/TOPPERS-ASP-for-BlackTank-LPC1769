/**
 * @page development 開発に関するトピック
 *
 * @section debugging デバッグ方法
 *
 * @subsection debugging_with_telnet telnetを使ったデバッグ
 *
 * @code
 * > telnet localhost 4444
 * >> halt
 * >> flash probe 0
 * >> flash write_image erase TOPPERS-ASP_BlackTank-LPC1769.bin
 * >> reset
 * @endcode
 *
 * @subsection debugging_with_gdb gdbを使ったデバッグ
 *
 * @code
 * > arm-none-eabi-gdb TOPPERS-ASP_BlackTank-LPC1769.axf
 * (gdb) target remote localhost:3333
 * (gdb) monitor halt
 * (gdb) load TOPPERS-ASP_BlackTank-LPC1769.axf
 * (gdb) continue
 * @endcode
 *
 * リセットがうまく動作しない場合、monitor haltの代わりにmonitor soft_reset_haltを試して下さい。
 *
 * @subsection debugging_with_lpc21isp lpc21ispを使った書き込み
 *
 * @code
 * > lpc21isp TOPPERS-ASP_BlackTank-LPC1769.hex /dev/ttyUSB0 57600 4000
 * @endcode
 */

