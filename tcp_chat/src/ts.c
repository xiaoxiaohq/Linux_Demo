#include "demo_head.h"

static unsigned int ts_fd;

//打开触摸屏设备 
int open_ts(void)
{
	ts_fd = open("/dev/input/event0",O_RDWR);
	if(ts_fd == -1)
    {
        printf("打开设备错误\n");
        return -1;
    }
	
	return 0;
}

unsigned int k_flag = 0;
char k_val = '#';

//主菜单选择
void *ts_analog_keyboard(void *arg)
{
	struct input_event  xy;  
	int x,y,touch;
	while(1)
	{
		read(ts_fd,&xy,sizeof(xy));
		  
		if(xy.type == EV_ABS  && xy.code == ABS_X)
			x = xy.value;
		if(xy.type == EV_ABS  && xy.code == ABS_Y)
			y = xy.value;
		 
		if(xy.type == EV_KEY  && xy.code == BTN_TOUCH)
		{
			touch = xy.value;
			if (touch == 0)
			{
				//获取表情包
				if (k_val == '@' && c_flag == 2 && k_flag == 2)
					get_bmp(x, y);
				
				//聊天界面输入法
				else if (k_flag == 2 && c_flag == 2)
					get_keyvalue(x, y);
				
				
				
				//获取键值
				else if (k_flag == 4 || k_flag == 5)
					get_keyvalue(x, y);
				//聊天界面控制
				else if (k_flag == 2)
					chat_ctrl(x, y);
				
				
				//关机
				else if (x > 700 && x < 800 && y > 0 && y < 50) 
					k_flag = 1;
				//登录
				else if (x > 100 && x < 210 && y > 370 && y < 430) 
					k_flag = 2;
				//注册
				else if (x > 590 && x < 700 && y > 370 && y < 430) 
					k_flag = 3;
				//用户
				else if (x > 260 && x < 610 && y > 110 && y < 160) 
					k_flag = 4;
				//密码
				else if (x > 260 && x < 610 && y > 200 && y < 250) 
					k_flag = 5;
			}
		}	
	}
}


int bmp_flag = -1;

int get_bmp(int _x, int _y)
{
	// if (_x > 700 && _x < 800 && _y > 0 && _y < 50)  //退出
		// bmp_flag = 0;
	if (_x > 30 && _x <105  && _y > 415 && _y < 470)
		bmp_flag = 0;
	else
	{
		if (_x < 10 || _y < 10)
		{
			bmp_flag = -1;
			return -1;
		}
		
		_x -= 10;
		_y -= 10;
	
		_x /= 100;
		_y /= 100;
		
		if (_x > 6 || _y > 2)
		{
			bmp_flag = -1;
			return -1;
		}
		
		bmp_flag = _x + _y*6 + 1;   //从第一张开始数
		
	}
	
	return 0;
}

//k_flag = 6 停止位

int get_keyvalue(int _x, int _y)
{
	//退出
	if ((_x > 660 && _x < 760 && _y > 350 && _y < 400) || (_x > 705 && _x < 760 && _y > 280 && _y < 350))
	{
		if (k_flag == 4 || k_flag == 5)	//密码账号退出
			k_flag = 6;
		else if(c_flag == 2)			//聊天退出
			c_flag = -2;
	}
		
	// y 220 --- 275
	else if (_x > 30 && _x < 85 && _y > 220 && _y < 275)
		k_val = '1';
	else if (_x > 95 && _x < 150 && _y > 220 && _y < 275)
		k_val = '2';
	else if (_x > 160 && _x < 215 && _y > 220 && _y < 275)
		k_val = '3';
	else if (_x > 225 && _x < 280 && _y > 220 && _y < 275)
		k_val = '4';
	else if (_x > 290 && _x < 345 && _y > 220 && _y < 275)
		k_val = '5';
	else if (_x > 355 && _x < 410 && _y > 220 && _y < 275)
		k_val = '6';
	else if (_x > 420 && _x < 475 && _y > 220 && _y < 275)
		k_val = '7';
	else if (_x > 485 && _x < 545 && _y > 220 && _y < 275)
		k_val = '8';
	else if (_x > 555 && _x < 610 && _y > 220 && _y < 275)
		k_val = '9';
	else if (_x > 620 && _x < 675 && _y > 220 && _y < 275)
		k_val = '0';
	else if (_x > 685 && _x < 765 && _y > 220 && _y < 275)   //退格
		k_val = '-';
	// y 285 --- 340
	else if (_x > 45 && _x < 100 && _y > 285 && _y < 340)   
		k_val = 'q';
	else if (_x > 110 && _x < 165 && _y > 285 && _y < 340)   
		k_val = 'w';
	else if (_x > 175 && _x < 235 && _y > 285 && _y < 340)   
		k_val = 'e';
	else if (_x > 245 && _x < 300 && _y > 285 && _y < 340)   
		k_val = 'r';
	else if (_x > 310 && _x < 365 && _y > 285 && _y < 340)   
		k_val = 't';
	else if (_x > 375 && _x < 430 && _y > 285 && _y < 340)   
		k_val = 'y';
	else if (_x > 440 && _x < 495 && _y > 285 && _y < 340)   
		k_val = 'u';
	else if (_x > 505 && _x < 565 && _y > 285 && _y < 340)   
		k_val = 'i';
	else if (_x > 570 && _x < 630 && _y > 285 && _y < 340)   
		k_val = 'o';
	else if (_x > 635 && _x < 695 && _y > 285 && _y < 340)   
		k_val = 'p';
	// y 350 --- 405
	else if (_x > 65 && _x < 120 && _y > 350 && _y < 405)   
		k_val = 'a';
	else if (_x > 130 && _x < 190 && _y > 350 && _y < 405)   
		k_val = 's';
	else if (_x > 200 && _x < 255 && _y > 350 && _y < 405)   
		k_val = 'd';
	else if (_x > 265 && _x < 320 && _y > 350 && _y < 405)   
		k_val = 'f';
	else if (_x > 330 && _x < 385 && _y > 350 && _y < 405)   
		k_val = 'g';
	else if (_x > 395 && _x < 450 && _y > 350 && _y < 405)   
		k_val = 'h';
	else if (_x > 460 && _x < 515 && _y > 350 && _y < 405)   
		k_val = 'j';
	else if (_x > 525 && _x < 580 && _y > 350 && _y < 405)   
		k_val = 'k';
	else if (_x > 590 && _x < 650 && _y > 350 && _y < 405)   
		k_val = 'l';
	// y 415 --- 470
	else if (_x > 115 && _x < 170 && _y > 415 && _y < 470)   
		k_val = 'z';
	else if (_x > 180 && _x < 240 && _y > 415 && _y < 470)   
		k_val = 'x';
	else if (_x > 245 && _x < 305 && _y > 415 && _y < 470)   
		k_val = 'c';
	else if (_x > 310 && _x < 370 && _y > 415 && _y < 470)   
		k_val = 'v';
	else if (_x > 380 && _x < 435 && _y > 415 && _y < 470)   
		k_val = 'b';
	else if (_x > 445 && _x < 500 && _y > 415 && _y < 470)   
		k_val = 'n';
	else if (_x > 510 && _x < 565 && _y > 415 && _y < 470)   
		k_val = 'm';
	else if (_x > 705 && _x < 765 && _y > 415 && _y < 470)   //清空
		k_val = '+';
	
	else if (_x > 30 && _x <105  && _y > 415 && _y < 470)
		k_val = '@';
}


int c_flag = -1;    //聊天界面标志位

int chat_ctrl(int _x, int _y)
{
	if (_x > 700 && _x < 800 && _y > 0 && _y < 50) 		//退出
		c_flag = 0;
		
	else if (_x > 55 && _x < 180 && _y > 420 && _y < 470) //更新好友
		c_flag = 1;
	else if (_x > 625 && _x < 750 && _y > 420 && _y < 470)  //聊天
		c_flag = 2;
		
	//ip触摸判断
	else
	{
		ts_get_ip_pos(_x, _y);
	}		
	
	return 0;
}

// 位置      c_flag
// 1           3
// 2           4
// 3           5
int ts_get_ip_pos(int _x, int _y)
{
	int offset = 0;
	while(1)
	{
		if (_x > 30 && _x < 280 && _y+offset > 75 && _y+offset < 100)
			c_flag = 3 + offset/30;
		
		offset += 30;
		if (offset > 300)
			break;	
	}
	
	return 0;
}


int close_ts(void)
{
	close(ts_fd);
}
