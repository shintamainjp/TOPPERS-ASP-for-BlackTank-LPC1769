#!/usr/bin/expect

#
# telnet経由でプログラムをライトする。
# こういうのはexpectを使えば良い。
#

set timeout 600
spawn telnet localhost 4444

expect ">"
send "halt\n"

expect ">"
send "flash probe 0\n"

expect ">"
send "flash write_image erase TOPPERS-ASP_BlackTank-LPC1769.bin\n"

expect ">"
send "exit\n"

sleep 2

spawn telnet localhost 4444

sleep 2

expect ">"
send "reset\n"

expect ">"
send "exit\n"
