#include "demo_head.h"

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
		chat_menu_show_picture(pic_buf, width, height, num);
		return;
	}
	if (flag == 3)
	{
		keyboard_show_pic_bmp(pic_buf, width, height, num);
		return;
	}
	if (flag == 4)
	{
		head_show_pic_bmp(pic_buf, width, height, num);
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

void head_show_pic_bmp(int *pic_buf, int width, int height, int num)
{
	int i,j;

	for (i=0; i<width; i++)
	{
		for (j=0; j<height; j++)
		{
			lcd_p[(i+15)+(j+num*35+75)*800] = pic_buf[i+j*width];
		}
	}
	
}



void keyboard_show_pic_bmp(int *pic_buf, int width, int height, int num)
{
	int i,j;
	int x, y;
	int m = num/6;
	int n = num%6;
	
	y = m*100 + 10;
	x = n*100 + 10;
	
	for (i=0; i<width; i++)
	{
		for (j=0; j<height; j++)
		{
			lcd_p[i+x+(j+y)*800] = pic_buf[i+j*width];
		}
	}
}


extern int offset_y;

void chat_menu_show_picture(int *pic_buf, int width, int height, int num)
{
	int i,j;
	int offset;
	if (num == 1)
		offset = 670;
	else if (num == 2)
		offset = 310;
	
	for (i=0; i<width; i++)
	{
		for (j=0; j<height; j++)
		{
			lcd_p[(i+offset)+(j+offset_y+70)*800] = pic_buf[i+j*width];
		}
	}
	
}

//无特效
void show_picture(int *pic_buf, int width, int height)
{
	int i,j;
	for(i=0; i<width; i++)
	{
		for(j=0; j<height; j++)
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
