#ifndef HEMO_HEAD_H
#define HEMO_HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/mman.h>   
#include <linux/input.h>
#include <dirent.h>
#include <string.h>

extern char pic_buf[128][1024];
extern int pic_cnt;
extern int num;
extern int buf_game[4][4];
extern int gamescore;
struct node 
{
	char data[1024];
	struct node *prev;
	struct node *next;
};

struct node *Create_node(char *filename);
void Create_and_insert_node(struct node *head, char *filename);
struct node * Array_end_of_linklist(struct node *head);

void show_pic(void);	
void pic_function(int tsfd, struct node *head);

int DIR_callback(struct node *head, char *dirname);

void show_one_bmp(char * filename, int flag, int num);
int read_JPEG_file (char * filename, int flag);
void clean(void);

int ts_main_choice(int tsfd);
int ts_function(int tsfd);
int ts_game_function(int tsfd);

void show_special_pic(int *pic_buf, int width, int height, int flag, int num);
void show_picture(int *pic_buf, int width, int height);
void show_crosswise_window_shades(int *pic_buf, int width, int height);
void show_lengthways_window_shades(int *pic_buf, int width, int height);
void  show_double_X(int *pic_buf, int width, int height);
void  show_double_Y(int *pic_buf, int width, int height);
void show_2048picture(int *pic_buf, int num);
void show_game_score(int *pic_buf, int num);

void data_refresh(void);
int game_function(int tsfd);
#endif

