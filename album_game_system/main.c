#include "demo_head.h"

unsigned int *lcd_p=NULL;
struct node *list = NULL;
int buf_game[4][4] = {0};
int gamescore = 0;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <dirpath>\n", argv[0]);
		exit(1);
	}
	
	list = Create_node("./main.jpg");
	struct node *head = list;
	
	//1.对LCD 设备进行映射 
	int lcd_fd=open("/dev/fb0",O_RDWR);
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
	
	DIR_callback(list, argv[1]);
	
	if (list->next == NULL)
	{
		read_JPEG_file ("error.jpg", 1);
		return -1;
	}
	list = list->next;
	
	//打开触摸屏设备 
	int ts_fd = open("/dev/input/event0",O_RDWR);
	if(ts_fd == -1)
    {
        printf("打开设备错误\n");
        return -1;
    }
	
	int num = 0;
	data_refresh();
	while(1)
	{
		//显示主界面
		clean();
		read_JPEG_file(head->data, 1);
	
		num = ts_main_choice(ts_fd);
		switch(num)
		{
			case 1:	//相册功能
					pic_function(ts_fd, head);
				break;
			case 2:
					game_function(ts_fd);
				break;
			case 3:
			
				break;
			case 0:	//退出
					close(ts_fd);
					close(lcd_fd);
					return 0;
				break;
			default:
				break;
		}
	}
	

	return 0;
}

//相册功能
void pic_function(int tsfd, struct node *head)
{
	clean();
	show_pic();	
	
	int num = 0;
	while(1)
	{
		num = ts_function(tsfd);
		
		switch(num)
		{
			case 1:
			{
				list = list->next;
				if (list == NULL)
				{
					list = head->next;
				}
				show_pic();			
			}
			break;
			case 2:
			{
				list = list->prev;
				if (list == head)
				{
					list = Array_end_of_linklist(head);
				}
				show_pic();
			}
			break;
			case 0:
				return;
			break;
			default:
			break;
		}	
	}
}
//1 下一张
//2 上一张
void show_pic()
{
	if (strstr(list->data, ".bmp") )
	{
		clean();
		show_one_bmp(list->data, 0, 0);	
	}
	if (strstr(list->data, ".jpg") || strstr(list->data, ".jpeg"))
	{
		clean();
		read_JPEG_file (list->data, 0);
	}	
}
