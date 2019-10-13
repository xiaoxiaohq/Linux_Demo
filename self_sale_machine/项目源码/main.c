#include "demo_head.h"

struct list_head *head = NULL;
char ex_font[16] = {0};

int main()
{
	//打开触摸屏设备
	open_lcd();
	
	//打开触摸设备
	int ts_fd = open_ts();
	
	//初始化字库
	Init_font();
	//清空账单
	free_shopping_account();
	//
	read_yuan_font();
	//读取商品信息
	head = malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(head);
	get_products_msg();
	
	//开机界面
	open_menu(ts_fd);
	
	while(1)
	{
		//显示主界面
		show_one_bmp("./pic/main.bmp", 0, 0);
		
		int ret = ts_main_menu(ts_fd);
		switch(ret)
		{
			case 1: //浏览商品
				browse_products_function(ts_fd);
			break;
			case 2: //购物车
				show_shopping_cart_function(ts_fd);
			break;
			case 3: //销售记录
				sale_record_function(ts_fd);
			break;
			case 0: //退出
				show_one_bmp("./pic/close.bmp", 0, 0);
				exit(0);
			break;
			default:
			break;
		}
	}	
	show_one_bmp("./pic/close.bmp", 0, 0);
	return 0;
}











