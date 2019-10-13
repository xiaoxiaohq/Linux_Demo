#include "demo_head.h"
#include <time.h>

//flag = 2 
//flag = 1 无特效
//flag = 0 有特效
void show_special_pic(int *pic_buf, int width, int height, int flag, int num)
{
	if (flag == 1)
	{
		show_picture(pic_buf, width, height);
		return;
	}
	if (flag == 2)
	{
		show_product_picture(pic_buf, num);
		return;
	}
	
	srand((unsigned int)time(NULL));
	int kind = rand()%4;
	if (kind == 0)
		show_lengthways_window_shades(pic_buf, width, height);
	if (kind == 1)
		show_crosswise_window_shades(pic_buf, width, height);	
	if (kind == 2)
		show_double_X(pic_buf, width, height);
	if (kind == 3)
		show_double_Y(pic_buf, width, height);
}

//无特效
void show_picture(int *pic_buf, int width, int height)
{
	int i,j;
	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			lcd_p[(i+(800-width)/2)+(j+(480-height)/2)*800] = pic_buf[i+j*width];
		}
	}
}

//纵向百叶窗
void show_lengthways_window_shades(int *pic_buf, int width, int height)
{
	int i,j;
	for(i=0;i<width/8;i++)
	{
		for(j=0;j<height;j++)
		{
			//*(lcd_p+(x+(800-width)/2)+(479-(y+(480-height)/2))*800) = color;
			lcd_p[(i+(800-width)/2)+(j+(480-height)/2)*800] 			= pic_buf[i+j*width];
			lcd_p[(i+(800-width)/2+width/8)+(j+(480-height)/2)*800] 	= pic_buf[(i+width/8)+j*width];
			lcd_p[(i+(800-width)/2+width/8*2)+(j+(480-height)/2)*800] 	= pic_buf[(i+width/8*2)+j*width];
			lcd_p[(i+(800-width)/2+width/8*3)+(j+(480-height)/2)*800] 	= pic_buf[(i+width/8*3)+j*width];	
			lcd_p[(i+(800-width)/2+width/8*4)+(j+(480-height)/2)*800] 	= pic_buf[(i+width/8*4)+j*width];
			lcd_p[(i+(800-width)/2+width/8*5)+(j+(480-height)/2)*800] 	= pic_buf[(i+width/8*5)+j*width];
			lcd_p[(i+(800-width)/2+width/8*6)+(j+(480-height)/2)*800] 	= pic_buf[(i+width/8*6)+j*width];
			lcd_p[(i+(800-width)/2+width/8*7)+(j+(480-height)/2)*800] 	= pic_buf[(i+width/8*7)+j*width];
		}
		usleep(2000);
	}
}

//横向百叶窗
void show_crosswise_window_shades(int *pic_buf, int width, int height)
{
	int i, j;
	for(j=0;j<height/6;j++)
	{
		for(i=0;i<width;i++) 
		{
			lcd_p[(i+(800-width)/2)+(j+(480-height)/2)*800] 				= pic_buf[i+j*width];
			lcd_p[(i+(800-width)/2)+((j+height/6)+(480-height)/2)*800] 		= pic_buf[i+(j+height/6)*width];
			lcd_p[(i+(800-width)/2)+((j+height/6*2)+(480-height)/2)*800] 	= pic_buf[i+(j+height/6*2)*width];
			lcd_p[(i+(800-width)/2)+((j+height/6*3)+(480-height)/2)*800] 	= pic_buf[i+(j+height/6*3)*width];
			lcd_p[(i+(800-width)/2)+((j+height/6*4)+(480-height)/2)*800] 	= pic_buf[i+(j+height/6*4)*width];
			lcd_p[(i+(800-width)/2)+((j+height/6*5)+(480-height)/2)*800] 	= pic_buf[i+(j+height/6*5)*width];			
		}
		usleep(2000);
	}
}

//
void  show_double_X(int *pic_buf, int width, int height)
{
	int x, y, py, ppy, pppy;

	for(y=height/4, py=y, ppy=3*y, pppy=ppy ; 
			(y>=0 || py<height/2 || ppy>=height/2 || pppy<height); 
				y--, py++, ppy--, pppy++)
	{
		if (y < 0)
			y = 0;
		if (py == height/2)
			py = height/2 - 1;
		if (ppy < height/2)
			ppy = height/2;
		if (pppy == height)
			pppy = height - 1;
		for(x = 0; x < width; x++)
		{
			lcd_p[(x+(800-width)/2)+(y+(480-height)/2)*800]  	= pic_buf[x+y*width];	
			lcd_p[(x+(800-width)/2)+(py+(480-height)/2)*800]  	= pic_buf[x+py*width];
			lcd_p[(x+(800-width)/2)+(ppy+(480-height)/2)*800]  	= pic_buf[x+ppy*width];	
			lcd_p[(x+(800-width)/2)+(pppy+(480-height)/2)*800]  = pic_buf[x+pppy*width];
		}
		usleep(1000);
	}
		
}

void  show_double_Y(int *pic_buf, int width, int height)
{
	int x, px,ppx, pppx, y;

	for(x=width/4, px=x, ppx=3*x, pppx=ppx; 
			(x>=0 || px<width/2 || ppx>=width/2 || pppx<width); 
				x--, px++, ppx--, pppx++)
	{
		if (x < 0)
			x = 0;
		if (px == width/2)
			px = width/2 - 1;
		if (ppx < width/2)
			ppx = width/2;
		if (pppx == width)
			pppx = width - 1;
		for(y = 0; y < height; y++)
		{
			lcd_p[(x+(800-width)/2)+(y+(480-height)/2)*800]  	= pic_buf[x+y*width];	
			lcd_p[(px+(800-width)/2)+(y+(480-height)/2)*800]  	= pic_buf[px+y*width];
			lcd_p[(ppx+(800-width)/2)+(y+(480-height)/2)*800]  	= pic_buf[ppx+y*width];	
			lcd_p[(pppx+(800-width)/2)+(y+(480-height)/2)*800]  = pic_buf[pppx+y*width];
		}
		
		usleep(1500);
	}
		
}

//商品图片
void show_product_picture(int *pic_buf, int num)
{
	int start_x, start_y;
	switch(num)
	{
		case 1:
			start_x = 0;
			start_y = 0;
		break;
		case 2:
			start_x = 200;
			start_y = 0;
		break;
		case 3:
			start_x = 400;
			start_y = 0;
		break;
		case 4:
			start_x = 0;
			start_y = 240;
		break;
		case 5:
			start_x = 200;
			start_y = 240;
		break;
		case 6:
			start_x = 400;
			start_y = 240;
		break;
		default :
		break;
	}
	
	int i,j;
	for(j=0;j<200;j++)
	{
		for(i=0;i<200;i++)
		{
			lcd_p[(i+start_x)+(j+start_y)*800] = pic_buf[i+j*200];
		}
	}
	
	//打印价格
	char buf[32] = {0};
	sprintf(buf, "%s %s", ex_price, ex_font);
	Display_characterXX(start_x+50, start_y+200, strlen(buf)+1, buf, 2, 0x00000000);
}
