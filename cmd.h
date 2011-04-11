/**
 * @file cmd.h
 * @author Shinichiro Nakamura
 * @brief ナチュラルタイニーシェルタスクコマンドの実装
 * @details
 */

#ifndef _CMD_H_
#define _CMD_H_

void cmd_taskinfo(int argc, char **argv);
void cmd_audio(int argc, char **argv);
void cmd_lcd(int argc, char **argv);
void cmd_led(int argc, char **argv);
void cmd_mount(int argc, char **argv);
void cmd_ls(int argc, char **argv);
void cmd_cd(int argc, char **argv);
void cmd_trace(int argc, char **argv);
void cmd_exit(int argc, char **argv);

#endif
