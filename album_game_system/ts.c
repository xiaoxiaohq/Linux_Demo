#include "demo_head.h"

int open_ts()
{
	//1.打开触摸屏设备 
	int tsfd = open("/dev/input/event0",O_RDWR);
	if(tsfd == -1)
    {
        printf("打开设备错误\n");
        return -1;
    }
	
	return tsfd;
}

int ts_main_choice(int tsfd)
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
			if (touch == 0)
			{
				if (x > 100 && x < 300 && y > 60 && y < 180) //相册
					return 1;
				if (x > 500 && x < 700 && y > 60 && y < 180) //游戏
					return 2;
				if (x > 100 && x < 300 && y > 290 && y < 410) //3
					return 3;
				if (x > 500 && x < 700 && y > 290 && y < 410) //退出
					return 0;
				
			}
		}
		
	}
	
	return 0;
}

int ts_function(int tsfd)
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
				if((x-x0) > 0) //右移
					return 1;
				if ((x-x0) < 0)//左移
					return 2;
				
			}
		}
		
	}
	
	return -1;
}

int ts_game_function(int tsfd)
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
				if (x > 600 && x < 700 && y > 350 && y < 450)//退出
					return 0;
				if (x > 600 && x < 700 && y > 200 && y < 300)//重开
					return 5;
					
				if ((x-x0) > 100 && abs(y-y0) < 100) //右移
					return 1;
				if ((x0-x) > 100 && abs(y-y0) < 100)//左移
					return 2;
				if ((y0-y) > 100 && abs(x-x0) < 100) //上移
					return 3;
				if ((y-y0) > 100 && abs(x-x0) < 100)//下移
					return 4;
				
			}
		}
		
	}
	
	return -1;
}


int close_ts(int tsfd)
{
	close(tsfd);
}
