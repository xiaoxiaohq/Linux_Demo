#ifndef HEMO_HEAD_H
#define HEMO_HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

#include <semaphore.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#include <sys/mman.h>   
#include <linux/input.h>
#include <time.h>
#include <ctype.h>

#include "kernel_list.h"
#include "font.h"

#define SIZE 256


typedef struct 
{
	char picpath[512];
	struct list_head list;  //内核链表
}PIC;

extern bool recv_warn;  //当你没有连接对面，对面给你发消息时

//system
extern unsigned int k_flag;
extern char k_val;
extern int c_flag;
extern int bmp_flag;

//LCD
extern unsigned int *lcd_p;

//lcd.c
int open_lcd(void);
int close_lcd(void);

//TCP
int tcp_client(void);
int tcp_refulsh_member_list(void);
int tcp_msg_to_file(char *_msg);
int tcp_add_msg_to_txt(char *_msg);
int tcp_clean_msg_txt(void);
int tcp_send_msg_to_server(char *_msg);
void *tcp_cli_recv_msg(void *arg);
int tcp_close(void);


//system_init.c
int system_init(void);
int system_function(void);
int back_one_bytes(char *_str);
int insert_char_to_string(char *_str);
int show_keyboard_menu_font(char *_str);
int sys_clean_keyboard_font(char *_str);
int get_keyboard_string(char *_str);
int show_account_passwd_font(char *_str, int _flag);
int login_system(const char *_account);
int register_account(const char *_account, const char *_passwd);
int login_judge_account_is_correct(const char *_account, const char *_passwd);
char *sys_get_chat_member_ip(void);
int sys_reflush_member_to_lcd(char *_ip);
int sys_show_keyboard_bmp_pic(void);
int sys_keyboard_select_bmp(void);
int sys_get_bmp_path(char *_str, int _num);
int sys_reflush_chat_msg(void);
int sys_clean_msg_txt_msg(void);
int sys_chat_judge_bmp_num(FILE *fp, int n, int m);


//ts.c
int get_keyvalue(int _x, int _y);
int open_ts(void);
int close_ts(void);
void *ts_analog_keyboard(void *arg);
int ts_get_ip_pos(int _x, int _y);
int get_bmp(int _x, int _y);
int chat_ctrl(int _x, int _y);


//showbmp.c
void clean(void);
void show_one_bmp(char * filename, int flag, int num);
void clean_keyboard_font(void);


//picspecial.c
//flag = 1 无特效
//flag = 0 有特效
void show_special_pic(int *pic_buf, int width, int height, int flag, int num);
void show_picture(int *pic_buf, int width, int height);
void show_lengthways_window_shades(int *pic_buf, int width, int height);
void show_crosswise_window_shades(int *pic_buf, int width, int height);
void show_double_X(int *pic_buf, int width, int height);
void show_double_Y(int *pic_buf, int width, int height);
void chat_menu_show_picture(int *pic_buf, int width, int height, int num);
void keyboard_show_pic_bmp(int *pic_buf, int width, int height, int num);
void head_show_pic_bmp(int *pic_buf, int width, int height, int num);


int DIR_callback(PIC *head, char *dirname);
int picdir_free_list(PIC *head);









#endif
