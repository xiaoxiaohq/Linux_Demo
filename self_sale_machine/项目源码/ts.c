#include "demo_head.h"

//打开触摸屏设备 
int open_ts(void)
{

	int tsfd = open("/dev/input/event0",O_RDWR);
	if(tsfd == -1)
    {
        printf("打开设备错误\n");
        return -1;
    }
	
	return tsfd;
}

//产品信息显示
int ts_open_show(int tsfd)
{
	struct input_event  xy;  
	int x,y,x0,y0,touch;
	while(1)
	{
		read(tsfd,&xy,sizeof(xy));
		  
		if(xy.type == EV_ABS  && xy.code == ABS_X)
			x = xy.value;
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
			y = xy.value;
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 0)
			{
				// 290---500   90--430   
				if (x > 290 && x < 500 && y < 430 && y > 90)
					return 1;
			}
		}	
	}
	
	return 0;
}


//主菜单选择
int ts_main_menu(int tsfd)
{
	struct input_event  xy;  
	int x,y,x0,y0,touch;
	while(1)
	{
		read(tsfd,&xy,sizeof(xy));
		  
		if(xy.type == EV_ABS  && xy.code == ABS_X)
			x = xy.value;
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
			y = xy.value;
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 0)
			{
				if (x > 50 && x < 200 && y > 250 && y < 350) //浏览商品
					return 1;
				if (x > 300 && x < 450 && y > 250 && y < 350) //购物车
					return 2;
				if (x > 550 && x < 700 && y > 250 && y < 350) //销售记录
					return 3;
				if (x > 700 && x < 800 && y > 0 && y < 100) //退出
					return 0;	
			}
		}	
	}
	
	return 0;
}

//浏览 切换界面
int ts_browse_menu(int tsfd)
{
	struct input_event  xy;  
	int x,y,x0,y0,touch;
	while(1)
	{
		read(tsfd,&xy,sizeof(xy));
		  
		if(xy.type == EV_ABS  && xy.code == ABS_X)
			x = xy.value;
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
			y = xy.value;
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 1)
			{
				x0 = x;
				y0 = y;
			}
			if (touch == 0)
			{
				if (x > 700 && x < 800 && y > 0 && y < 100)  //退出
					return 0;	
				
				if (x > 670 && x < 800 && y > 380 && y < 480) //购物车
					return 3;
					
					
				if (abs(x-x0) < 100 && (y0-y) > 200) //上划
					return 1;
				if (abs(x-x0) < 100 && (y-y0) > 200) //下滑
					return 2;
				
				if (x > 650 && x < 750 && y > 140 && y < 240) //上一张
					return 1;
				if (x > 650 && x < 750 && y > 240 && y < 340) //下一张
					return 2;
					
				
				
				if (x > 0 && x < 200 && y > 0 && y < 240) //1
					return 4;
				if (x > 200 && x < 400 && y > 0 && y < 240) //2
					return 5;
				if (x > 400 && x < 600 && y > 0 && y < 240) //3
					return 6;
				if (x > 0 && x < 200 && y > 240 && y < 480) //4
					return 7;
				if (x > 200 && x < 400 && y > 240 && y < 480) //5
					return 8;
				if (x > 400 && x < 600 && y > 240 && y < 480) //6
					return 9;
				
			}
		}	
	}
	
	return 0;
}

//产品信息显示
int ts_product_show(int tsfd)
{
	struct input_event  xy;  
	int x,y,x0,y0,touch;
	while(1)
	{
		read(tsfd,&xy,sizeof(xy));
		  
		if(xy.type == EV_ABS  && xy.code == ABS_X)
			x = xy.value;
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
			y = xy.value;
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 0)
			{
				if (x > 700 && x < 800 && y > 0 && y < 100)  //退出
					return 0;	
				
				//确定
				//430  500     250  300      
				//150           90
				if (x > 580 && x < 650 && y > 340 && y < 390)
					return 3;
				if (x > 520 && x < 570 && y > 250 && y < 290) //加
					return 1;
				if (x > 410 && x < 460 && y > 250 && y < 290) //减
					return 2;	
			}
		}	
	}
	
	return 0;
}


/*
	75  80        65    95
	120  125      110   140
	165   170      155   185
    210    215     200    235

*/

/*
	删除
		30  70    65    80
				  110    125
				  155     170
 


*/
int set_ts_pos_shopping_cart(int _x, int _y)
{
	int offset = 0;
	int level = 0;
	while(1)
	{
		if (_x > 440 && _x < 490 && _y > 65+offset && _y < 95+offset) //-
		{
			return (2+level);
		}
		else if (_x > 540 && _x < 590 && _y > 65+offset && _y < 95+offset) //+
		{
			return (3+level);
		}
		else if(_x > 30 && _x < 70 && _y > 65+offset && _y < 80+offset ) //删除
		{
			return (4+level);
		}
		else
		{
			offset += 45;
			level += 3;
			if (offset >= 450)
				break;
		}				
	}
}
//购物车
int ts_shopping_cart(int tsfd)
{
	struct input_event  xy;  
	int x,y,x0,y0,touch;
	while(1)
	{
		read(tsfd,&xy,sizeof(xy));
		  
		if(xy.type == EV_ABS  && xy.code == ABS_X)
			x = xy.value;
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
			y = xy.value;
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 0)
			{
				if (x > 700 && x < 800 && y > 0 && y < 100)  //退出
					return 0;	
				
				
				if (x > 700 && x < 800 && y > 400 && y < 480) //结账
					return 1;
					
				if (x > 0 && x < 100 && y > 0 && y < 80) //管理
					return 2;
				//int ret = set_ts_pos_shopping_cart(x, y); //+-判断
				//return ret;
			}
		}	
	}
	
	return 0;
}

//购物车管理
int ts_manage_shopping_cart(int tsfd)
{
	struct input_event  xy;  
	int x,y,x0,y0,touch;
	while(1)
	{
		read(tsfd,&xy,sizeof(xy));
		  
		if(xy.type == EV_ABS  && xy.code == ABS_X)
			x = xy.value;
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
			y = xy.value;
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 0)
			{
				if (x > 700 && x < 800 && y > 0 && y < 100)  //退出
					return 0;	
				
				if (x > 0 && x < 100 && y > 0 && y < 60)  //保存
					return 1;
					
				int ret = set_ts_pos_shopping_cart(x, y); //+-判断
				return ret;
			}
		}	
	}
	
	return 0;
}

int ts_amount_function(int tsfd)
{ 
	struct input_event  xy;  
	int x,y;
	int x0,y0;
	int touch;
	while(1)
	{
		read(tsfd,&xy,sizeof(xy));
		 
		if(xy.type == EV_ABS  && xy.code == ABS_X)
		{
			x = xy.value;
		}
		 
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
		{
			y = xy.value;
		}
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 1)
			{
				x0 = x;
				y0 = y;
			}
			if (touch == 0)
			{
				if (x > 700 && x < 800 && y > 0 && y < 100)//退出
					return 0;
			
			}
		}
		
	}
	return -1;
}

//销售记录查看
int ts_show_sale_account(int tsfd)
{
	struct input_event  xy;  
	int x,y,x0,y0,touch;
	while(1)
	{
		read(tsfd,&xy,sizeof(xy));
		  
		if(xy.type == EV_ABS  && xy.code == ABS_X)
			x = xy.value;
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
			y = xy.value;
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 1)
			{
				x0 = x;
				y0 = y;
			}
			if (touch == 0)
			{
				if (x > 700 && x < 800 && y > 0 && y < 100)  //退出
					return 0;	
						
				if (abs(x-x0) < 100 && (y0-y) > 200) //上划
					return 1;
				if (abs(x-x0) < 100 && (y-y0) > 200) //下滑
					return 2;
				
			}
		}	
	}
	
	return 0;
}

int close_ts(int tsfd)
{
	close(tsfd);
}
