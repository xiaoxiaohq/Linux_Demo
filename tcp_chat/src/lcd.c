#include "demo_head.h"

unsigned int *lcd_p = NULL;
static int lcd_fd;

//打开屏幕及映射
int open_lcd(void)
{
	//1.对LCD 设备进行映射 
	lcd_fd=open("/dev/fb0",O_RDWR);
	if(lcd_fd==-1)
    {
        printf("打开设备错误\n");
        return -1;
    }
		
    lcd_p = (unsigned int *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(lcd_p == MAP_FAILED)
	{
		perror("mmap call failed: ");
		return -1;
	}
	
	return 0;
}


int close_lcd(void)
{
	int ret = munmap(lcd_p, 800*480*4);
	if (ret == -1)
	{
		perror("munmap failed");
		return -1;
	}
	
	close(lcd_fd);
	
	return 0;
}