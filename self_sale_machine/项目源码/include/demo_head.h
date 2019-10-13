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
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "list.h"
#include "font.h"

typedef struct node 
{
	char name[1024];	 //商品名
	char price[128];	 //商品价格
	int num;		 		//商品数量
	char filepath[1024]; //商品路径
	
	int saleNum; //出售数量
	struct list_head list;
}Node;

struct proXY
{
	int x;
	int y;
};

extern unsigned int *lcd_p;
extern struct list_head *head;
extern char * ex_price;
extern char ex_font[16];

int open_lcd(void);
int open_ts(void);
int retell_product_txt(void);
void read_yuan_font(void);

int ts_open_show(int tsfd);
int ts_main_menu(int tsfd);
int ts_browse_menu(int tsfd);
int ts_product_show(int tsfd);
int ts_shopping_cart(int tsfd);
int ts_amount_function(int tsfd);
int ts_manage_shopping_cart(int tsfd);
int ts_show_sale_account(int tsfd);

int open_menu(int tsfd);

void sale_record_function(int tsfd);
void show_shopping_cart_function(int tsfd);
void browse_products_function(int tsfd);
void get_products_msg(void);

struct list_head *show_product_pic(struct list_head *new);
struct list_head *go_forward_six(struct list_head *new, int step);
struct list_head *locate_node(struct list_head *tmp, int step);
Node *get_product_form_list(struct list_head *tmp);
void show_product_msg(Node *tmp);

void show_sale_account_time_font(int offset);
void show_sale_font(int offset, char *name, char *price, int num, float money);
int show_sale_font_to_lcd(FILE *fp);
int save_shopping_cart_txt(Node *tmp, int pnum);
void free_sale_account(void);
void save_account_to_sale(void);

struct proXY * show_font_XY(int level);
char* itoa(int num,char* str,int radix);
void show_font_product(Node *tmp, int _num);
void show_font_product_refresh(Node *tmp, int _num);

void show_account_font_refresh(int offset, char *name, char *price, int num);
void show_shopping_product_account_refresh(void);
void show_shopping_product_account(void);
void free_counter(char *_name);
int get_shopping_cart_add_del(int level);
int manage_shopping_cart(int tsfd);

void show_shopping_product_account_refresh_manage(void);
int show_shopping_kind_font(void);
void show_amount_money(int tsfd);
void show_account_font_manage(int offset);
void show_account_font(int offset, char *name, char *price, int num);
int free_shopping_account_zero(void);
void free_shopping_account(void);
Node *get_product_for_modify(char *_name);

void show_one_bmp(char * filename, int flag, int num);
void clean(void);
void clean_money(void);
void clean_manage_font(void);
void insert_node_to_list_sort(Node *tmp);


void show_product_picture(int *pic_buf, int num);
void show_special_pic(int *pic_buf, int width, int height, int flag, int num);
void show_picture(int *pic_buf, int width, int height);
void show_crosswise_window_shades(int *pic_buf, int width, int height);
void show_lengthways_window_shades(int *pic_buf, int width, int height);
void  show_double_X(int *pic_buf, int width, int height);
void  show_double_Y(int *pic_buf, int width, int height);
#endif

