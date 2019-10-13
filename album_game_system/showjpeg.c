#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <unistd.h>

#include "demo_head.h"

//声明为外部变量
extern   unsigned int *lcd_p;

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
	{
	  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	  my_error_ptr myerr = (my_error_ptr) cinfo->err;

	  /* Always display the message. */
	  /* We could postpone this until after returning, if we chose. */
	  (*cinfo->err->output_message) (cinfo);

	  /* Return control to the setjmp point */
	  longjmp(myerr->setjmp_buffer, 1);
}

//GLOBAL(int)
int read_JPEG_file (char * filename, int flag)
{
	 
	 //1.定义JPEG 解码对象
	  struct jpeg_decompress_struct cinfo;
	 
 
	 //2.定义一个JPEG 解码出错对象 
	  struct my_error_mgr jerr;
	  /* More stuff */  		  /*源文件*/
	  JSAMPARRAY buffer;		/*输出行缓存*/
	  int row_stride;		/* 输出行缓存的长度*/

		FILE *infile=NULL;

			//打开文件 
	  if ((infile = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		return 0;
	  }

	  /*初始化出错对象 */
	  cinfo.err = jpeg_std_error(&jerr.pub);
	  jerr.pub.error_exit = my_error_exit;
	  /* Establish the setjmp return context for my_error_exit to use. */
	  if (setjmp(jerr.setjmp_buffer)) 
	  {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 0;
	  }
  
	/* 现在我们可以初始化JPEG 的解码对象 */
	jpeg_create_decompress(&cinfo);

	/*关联解码对象与源文件 */
	jpeg_stdio_src(&cinfo, infile);

	/*读取JPEG的头数据 */
	(void) jpeg_read_header(&cinfo, TRUE);
 
	/*开始解码*/
	(void) jpeg_start_decompress(&cinfo);
 
	int image_width = cinfo.output_width;
	int image_height = cinfo.output_height;
 
	double tran_width = 800.00 / (double)image_width;
	double tran_height = 480.00 / (double)image_height;
	
	//大图片满屏显示
	if (image_width > 800 || image_height > 480)
	{
		int pre_i,pre_j,after_i,after_j;//缩放前对应的像素点坐标
		
		//求出一行数据所占用的字节数
		row_stride = cinfo.output_width * cinfo.output_components;
		/*根据行宽分配的堆空间 */
		buffer = (*cinfo.mem->alloc_sarray)   //函数回调 
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

		unsigned int bufferbig [image_width*image_height];
		
		//重点需要操作 与 理解的部分  
		while (cinfo.output_scanline < cinfo.output_height) 
		{
			(void) jpeg_read_scanlines(&cinfo, buffer, 1);   //把一行 JPEG 数据解码成 RGB 放到BUF 中  
				
			//记录当前行
			int y= cinfo.output_scanline - 1;
			char *p  = buffer[0];  
				
			int x=0;
			for(x=0; x<image_width; x++)
			{
				char R = *p++;
				char G = *p++;
				char B = *p++;
				char A = 0; 
					
				int color  = A << 24 | R <<16 | G << 8 | B; 

				bufferbig[y*image_width + x]  = color;
				
			}
		}
		
		//满屏显示
		//坐标变换
		//将原图按比例取像素到新图中
		int tmp_buf[800*480] = {0};
		int i, j;
		for(i = 0;i < 480;i++)
		{
			for(j = 0;j < 800;j++)
			{
				after_i = i;
				after_j = j;
				
				pre_i = (int)(after_i / tran_height);
				pre_j = (int)(after_j / tran_width);
				
				//*(lcd_p+i*800 + j) = bufferbig[pre_i * image_width + pre_j];
				tmp_buf[i*800 + j] = bufferbig[pre_i * image_width + pre_j];
			}
		}
		//显示图片
		show_special_pic(tmp_buf, 800, 480, flag, 0);
	}
	else //小图片中间显示
	{
		//求出一行数据所占用的字节数
		row_stride = cinfo.output_width * cinfo.output_components;
		/*根据行宽分配的堆空间 */
		buffer = (*cinfo.mem->alloc_sarray)   //函数回调 
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	 
		int tmp_buf[image_width*image_height];	
		//重点需要操作 与 理解的部分  
		while (cinfo.output_scanline < cinfo.output_height) 
		{
			(void) jpeg_read_scanlines(&cinfo, buffer, 1);   //把一行 JPEG 数据解码成 RGB 放到BUF 中  
				
			//记录当前行
			int y= cinfo.output_scanline - 1;
	
			char *p  = buffer[0];  
			
			int x=0;
			for(x=0; x<image_width; x++)
			{
				char R = *p++;
				char G = *p++;
				char B = *p++;
				char A = 0; 
					
				int color  = A << 24 | R <<16 | G << 8 | B; 

				tmp_buf[y*image_width + x]  = color;
				//*(lcd_p+(x+(800-image_width)/2)+(y+(480-image_height)/2)*800) = color;	
			}
		}
		
		//显示图片
		show_special_pic(tmp_buf, image_width, image_height, flag, 0);		
	}
	
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 1;
}
