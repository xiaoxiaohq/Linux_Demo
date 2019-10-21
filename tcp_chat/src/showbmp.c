#include "demo_head.h"

extern unsigned int *lcd_p;

//清屏函数
void clean(void)
{
	 int i=0;
	 for(i=0;i<800*480;i++)
	 {
		 *(lcd_p+i) = 0x00000000;
	 }
}

void clean_keyboard_font(void)
{
	int i=0, j=0;
	for(j=20; j<100; j++)
	for(i=20;i<600;i++)
	{
		 *(lcd_p+i+j*800) = 0x00ffffff;
	}
}


void show_one_bmp(char * filename, int flag, int num)
{
	int pic_fd = open(filename,O_RDWR);
	if(pic_fd < 0)
	{
		perror("open failed");
		return;
	}
	
	//读取BMP 图片的头数据
	char head[54]={0};
	read(pic_fd,head,54);

	//获取图片的头数据    
	int  width =  *((int *)&head[18]); //宽度  / 位图的宽度，以像素为单位(18-21字节)
	int  height =  *((int *)&head[22]); // 位图的高度，以像素为单位(22-25字节)
	
	//printf("width = %d, height = %d\n", width, height);
	short  bit =  *((short *)&head[28]);  //(28-29字节)
	
	//压缩倍数
	double tran_width = 800.00 / (double)width;
	double tran_height = 480.00 / (double)height;
	
	//图片大于显示屏时，满屏显示
	if (width > 800 || height > 480)//缩小图片
	{
		//压缩前每行字节数
		int step = 3 * width;
		//压缩后每行字节数
		int Sca_step = 3 * 800;
		int i, j, k;
		
		int pre_i,pre_j,after_i,after_j;//缩放前对应的像素点坐标
		//初始化缩放后图片的信息
		
		//读取原始数据
		char bufbig[width*height*3]; 	
		read(pic_fd,bufbig,width*height*3);
		
		//压缩后的像素
		width = 800;
		height = 480;
		
		//存放压缩后的数据
		char buffull[800*480*3]={0};
		
			
		//坐标变换
		//将原图按比例取像素到新图中
		for(i = 0;i < height;i++)
		{
			for(j = 0;j < width;j++)
			{
				after_i = i;
				after_j = j;
				
				pre_i = (int)(after_i / tran_height);
				pre_j = (int)(after_j / tran_width);
				for(k=0; k<3; k++)
				{
					buffull[i * Sca_step + j*3 +k] = bufbig[pre_i * step + pre_j*3 + k];
					//*(lcd_p+i * Sca_step + j*3 +k) = bufbig[pre_i * step + pre_j*3 + k];
				}
			}
		}
		//将24位转成32位
		
		char  *p = buffull; 
		int tmp_buf[width*height];
		int x=0,y=0; 		
		for(y=0;y<height;y++)
		{
			for(x=0;x<width;x++)
			{
				
				char B = *p++; 
				char G = *p++; 
				char R = *p++; 
				char A = 0; 
				
				//合并
				int color  = A << 24 | R << 16 | G << 8 | B; 
				
				//(480-height)/2
				//(800-width)/2
				//把合并后的颜色值放到 LCD 屏幕中 
				//*(lcd_p+x+(479-y)*800) = color;	
				tmp_buf[x+(height-1-y)*width] = color;
			}
		}
		//显示图片
		show_special_pic(tmp_buf, width, height, flag, num);
	}
	else	//小图片 屏幕中间显示
	{
		if(bit == 24)  //进行图片解码
		{
			char buf24[width*height*3]; 
			read(pic_fd,buf24,width*height*3);
			 
						
			char  *p = buf24;  			

			int tmp_buf[width*height];
			//获取RGB 数据 并 合并成 32位的RGB  
			int x=0,y=0; 			
			for (y=0; y<height; y++)
			{
				for (x=0; x<width; x++)
				{					
					char B = *p++; 
					char G = *p++; 
					char R = *p++; 
					char A = 0; 
					
					//合并
					int color  = A << 24 | R << 16 | G << 8 | B; 
					
					//(480-height)/2
					//(800-width)/2
					//把合并后的颜色值放到 LCD 屏幕中 
					//*(lcd_p+(x+(800-width)/2)+(479-(y+(480-height)/2))*800) = color;
					tmp_buf[x+(height-1-y)*width] = color;
				}
			}
			//显示图片
			show_special_pic(tmp_buf, width, height, flag, num);
		}		
		close(pic_fd);
	}
}