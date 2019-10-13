#ifndef	__font__
#define	__font__

//初始化字库 
int Init_font(void);

//销毁字库
void  UnInit_Lcd(void);

//显示汉字
int  Display_characterXX(unsigned int x,  //显示字体的X轴
						unsigned int y, //显示字体的Y轴
						unsigned int len, //字体的长度
						unsigned char *string, //需要显示的汉字
						int size, //汉字的大小  1->8倍
						unsigned long color); //汉字的颜色

#endif