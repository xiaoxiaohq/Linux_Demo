#include "demo_head.h"

static PIC *head;
extern char *sendip;
static bool bmppack_flag = false;

int system_init(void)
{
	//打开触摸屏设备
	open_lcd();
	
	//打开触摸设备
	open_ts();
	
	//初始化字库
	Init_font();
	
	return 0;
}

int system_function(void)
{
	//创建一个线程
	pthread_t id;
	pthread_create(&id, NULL, ts_analog_keyboard, NULL);
	
	//显示界面
	clean();
	show_one_bmp("./open/open.bmp", 1, 0);
			
	//循环接收账号密码
	char *k_account = calloc(1, 16);
	bzero(k_account, 16);
	char *k_password = calloc(1, 16);
	bzero(k_password, 16);
	
	bool flag_flag = false;
	while(1)
	{
		//关机
		if (k_flag == 1)
			break;
		//登录
		else if (k_flag == 2)
		{
			int ret = login_judge_account_is_correct(k_account, k_password);  //判断账号密码是否正确
#ifdef DEBUG
				printf("[%d]\n", __LINE__);
#endif				
			if (ret == 0)
			{

#ifdef DEBUG
				printf("登录成功\n");
#endif				
			
				login_system(k_account);			//进入聊天界面	
				flag_flag = true;
				k_flag = 0;
				
				bzero(k_password, 16);
				// break;
			}
			else if (ret == 1) //账号不存在
			{
				show_one_bmp("./open/accounterr.bmp", 1, 0);
				sleep(1);
				flag_flag = true;
				k_flag = 0;
			}
			else if (ret == 2) //账号密码不全
			{
				show_one_bmp("./open/reglost.bmp", 1, 0);
				sleep(1);
				flag_flag = true;
				k_flag = 0;				
			}
			else if (ret == 3) //账号密码没有输入
			{
				show_one_bmp("./open/loginerr.bmp", 1, 0);
				sleep(1);
				flag_flag = true;
				k_flag = 0;
			}
			else if (ret == 4) //密码出错
			{
				show_one_bmp("./open/passwderr.bmp", 1, 0);
				sleep(1);
				flag_flag = true;
				k_flag = 0;
			}
										
		}
		//注册
		else if (k_flag == 3)
		{
			int ret = register_account(k_account, k_password);//将账号、密码，写入文本
			
			
			if (ret == 1) //账号密码不全
			{
				show_one_bmp("./open/reglost.bmp", 1, 0);
				sleep(1);
				flag_flag = true;
				k_flag = 0;
			}
			else if (ret == 2) //账号已存在
			{
				show_one_bmp("./open/regexist.bmp", 1, 0);
				sleep(1);
				flag_flag = true;
				k_flag = 0;
			}
			else if (ret == 0) //注册成功
			{
				show_one_bmp("./open/regsuccess.bmp", 1, 0);
				sleep(1);
				flag_flag = true;
				k_flag = 0;
			}
		}
		//用户名
		else if (k_flag == 4)
		{
			get_keyboard_string(k_account);			//获取用户名
#ifdef DEBUG
			printf("账号：%s\n", k_account);
#endif
			flag_flag = true;
		}	
		//密码
		else if (k_flag == 5)
		{
			get_keyboard_string(k_password);		//获取密码
#ifdef DEBUG
				printf("密码: %s\n", k_password);
#endif
			flag_flag = true;
		}
		
		
		if (flag_flag == true)
		{
			// 显示界面
			clean();
			show_one_bmp("./open/open.bmp", 1, 0);
		
			// 显示账号，密码
			show_account_passwd_font(k_account, 0);
			show_account_passwd_font(k_password, 1);
			
			k_flag = 0;		//清除标志位
			flag_flag = false;
		} 
		
			
	}
	show_one_bmp("./open/close.bmp", 1, 0);
	pthread_cancel(id);
	free(k_account);
	free(k_password);
	return 0;
}

int login_judge_account_is_correct(const char *_account, const char *_passwd)
{
	if (strlen(_account) == 0 && strlen(_passwd) == 0)
	{
		return 3;
	}

	if (strlen(_account) == 0 || strlen(_passwd) == 0)
	{
		return 2;
	}
	
	FILE *fp = fopen("./passwd.txt", "r");  //打开密码文本
	if (fp == NULL)
	{
		perror("fopen login failed");
		return -1;
	}
	
	char account[16];
	char passwd[16];

	while(1)
	{

		if (feof(fp))  	//账号不存在
			break;
	
		bzero(account, 16);
		bzero(passwd, 16);
#ifdef DEBUG
	printf("[%d]\n", __LINE__);
#endif
		fscanf(fp, "%s %s", account, passwd);
#ifdef DEBUG
	printf("[%d]\n", __LINE__);
#endif


		if (!strcmp(account, _account) && !strcmp(_passwd, passwd)) //登录
		{
			fclose(fp);
			return 0;
		}		
		else if (!strcmp(account, _account))  //密码出错
		{
			fclose(fp);
			return 4;
		}
	}
	
	fclose(fp);
	return 1;
}

int register_account(const char *_account, const char *_passwd)
{
	if (strlen(_account) == 0 || strlen(_passwd) == 0) //账号密码不全
	{		
#ifdef DEBUG
				printf("账号密码不全\n");
#endif
		return 1;
	}
	
	FILE *fp = fopen("./passwd.txt", "r+");
	if (fp == NULL)
	{
		perror("fopen register failed");
		return -1;
	}
	
	char account[16];
	char passwd[16];
	while(1)
	{
		if (feof(fp))      //到尾
			break;
		
		bzero(account, 16);
		bzero(passwd, 16);
		fscanf(fp, "%s %s", account, passwd);
		if (!strcmp(account, _account))  //账号重复
		{
#ifdef DEBUG
				printf("账号已存在\n");
#endif
			fclose(fp);
			return 2;
		}
	}
	
	//添加账号到文本
	fprintf(fp, "%s %s\n", _account, _passwd);
#ifdef DEBUG
				printf("账号注册成功\n");
#endif	
	fclose(fp);
	return 0;
}

//登录系统
int login_system(const char *_account)
{
	//打开TCP连接
	tcp_client();
	//界面显示
	clean();
	show_one_bmp("./login/chat.bmp", 1, 0);
	
	head = malloc(sizeof(PIC));
	INIT_LIST_HEAD(&head->list);
	bzero(head->picpath, 512);
	
	//获取表情包
	DIR_callback(head, "./pic");
#ifdef DEBUG
		printf("表情包读取完成\n");
#endif
	
	
	// 创建线程接收服务器发送的消息
	pthread_t id;
	pthread_create(&id, NULL, tcp_cli_recv_msg, NULL);
	
	char *send_msg = calloc(1, 256);
	bzero(send_msg, 256);
	bzero(sendip, 16);
	char *_ip = NULL;
	c_flag = -1;
	bool second_flag = false;
	while(1)
	{

		if (recv_warn == true && c_flag == -1)
		{
			recv_warn = false;
			
			//刷新界面
			clean();
			show_one_bmp("./login/chat.bmp", 1, 0);
			
			//好友显示
			if (_ip != NULL)
			{
				sys_reflush_member_to_lcd(_ip);
				Display_characterXX(350,  430, strlen(_ip)+1, _ip, 2, 0x00ff0000);
			}
				
			sys_reflush_chat_msg();   //聊天信息显示
		}
		//刷新好友列表
		if (c_flag == 1)
		{
			tcp_refulsh_member_list();
			show_one_bmp("./login/reflushlist.bmp", 1, 0);
			sleep(2);
			second_flag = true;
#ifdef DEBUG
			printf("刷新好友列表\n");
#endif
		}
		//聊天
		else if (c_flag == 2)
		{
			
			if (_ip == NULL)	//没有ip无法发送给，发消息的用户
			{
				recv_warn = true; //如果没有ip,让它直接显示信息，但无法发送信息
				c_flag = -1;
				continue;
			}
			recv_warn = false;
			
			second_flag = true;
			
			bmppack_flag = true;
			
			//获取按键字符
			get_keyboard_string(send_msg);
			
			if (send_msg != NULL && strlen(send_msg) > 0)
			{	
#ifdef DEBUG
				printf("获取到的键盘数据:%s\n", send_msg);
#endif
				//发送消息给服务器
				tcp_send_msg_to_server(send_msg);
				
				char c_tmp[256] = {0};
				sprintf(c_tmp, "my:%s", send_msg);
#ifdef DEBUG
				printf("保存到本地的数据：%s\n", c_tmp);
#endif
				//保存到本地文本
				tcp_add_msg_to_txt(c_tmp);
#ifdef DEBUG
				printf("数据保存成功\n");
#endif		
			}
			bmppack_flag = false;
		}
		//退出
		else if (c_flag == 0)
		{
			break;
		}
			
		else if (c_flag > 2)
		{
			//获取触摸聊天对象ip
			_ip = sys_get_chat_member_ip();
#ifdef DEBUG
			printf("获取到的聊天对象ip: %s\n", _ip);
#endif
			second_flag = true;
		}
		
		
		if (second_flag == true)
		{
			//刷新界面
			clean();
			show_one_bmp("./login/chat.bmp", 1, 0);
			
			
			if (c_flag != 2 && c_flag != 1)
			{
				tcp_clean_msg_txt();  //清空本地日记
				// bzero(_ip, strlen(_ip));
			}	
			else if(c_flag == 1 || c_flag == 2)
			{
				sys_reflush_chat_msg();   //聊天信息显示
			}
				
				
			//好友显示
			sys_reflush_member_to_lcd(_ip);


			//显示聊天对象
			if (c_flag >= 1 && _ip != NULL)
			{				
				Display_characterXX(350,  430, strlen(_ip)+1, _ip, 2, 0x00ff0000); 
			}
				
			
			c_flag = -1;
			second_flag = false;
		}
	}

#ifdef DEBUG
	printf("send_msg \n");
#endif	
	free(send_msg);
	// 撤销线程
	pthread_cancel(id);
#ifdef DEBUG
	printf("tcp \n");
#endif	

	//关闭TCP连接
	tcp_close();
#ifdef DEBUG
		printf("free [%d]\n", __LINE__);
#endif
	picdir_free_list(head);
	free(head);
#ifdef DEBUG
		printf("free [%d]\n", __LINE__);
#endif	
	//清空本地日记
	tcp_clean_msg_txt();  
	sys_clean_msg_txt_msg();
	
	return 0;
}


int sys_chat_judge_bmp_num(FILE *fp, int n, int m)
{
	rewind(fp);  //重头开始
	int num = m;
	char buf[SIZE];
	while(num)  //去掉多余项
	{
		bzero(buf, SIZE);
		fscanf(fp, "%s", buf);
		--num;
	}
	
	num = n - m;	//
	int n_flag = 0;
	while(num)
	{
		bzero(buf, SIZE);
		fscanf(fp, "%s", buf);
		if (strlen(buf) == 0)
			break;
		if (strstr(buf, ".bmp") != NULL)
			++n_flag;
		--num;
	}
	
	int ret;
	if (n_flag > 3 || (n_flag == 3 && (n-m) > 3) ||(n_flag == 2 && (n-m) > 5) || (n_flag == 1 && (n-m) > 7))
		ret = sys_chat_judge_bmp_num(fp, n, ++m);
	else
	{
		return n-m;
	}
	
	return ret;
}

int offset_y = 0;

int sys_reflush_chat_msg(void)
{
	offset_y = 0;
	int offset_x = 0;
	
	FILE *fp = fopen("./syslog.txt", "r");
	if (fp == NULL)
	{
		perror("chat fopen failed");
		return -1;
	}
	
	char buf[SIZE];
	int n = 0;
	int n_flag = 0;
	while(1)
	{
		if (feof(fp))
			break;
		
		bzero(buf, SIZE);
		fscanf(fp, "%s", buf);
		if (strlen(buf) == 0)
			break;
		n += 1;
	}
#ifdef DEBUG
	printf("[%d] 本地文件存在信息数目：%d\n", __LINE__, n);
#endif
	
	
	int m = 0;
	int ret;
	//计算图片数量
	if (n >= 9)
	{
		//还剩8条消息
		m = n - 9;
		ret = sys_chat_judge_bmp_num(fp, n, m);

		n -= ret;

		rewind(fp);
		while(n)
		{
			bzero(buf, SIZE);
			fscanf(fp, "%s", buf);
			--n;
		}
	}
	else if (n > 2 && n < 9)
	{
		m = 0;
		ret = sys_chat_judge_bmp_num(fp, n, m);
		
		n -= ret;
		rewind(fp);
		while(n)
		{
			bzero(buf, SIZE);
			fscanf(fp, "%s", buf);
			--n;
		}
		
	}
	else
	{
		rewind(fp);
	}
	
	//打印到LCD屏上
	while(1)
	{
		if (feof(fp))
			break;
		
		bzero(buf, SIZE);
		fscanf(fp, "%s", buf);
		if (strlen(buf) == 0)
			break;
		
		if (strstr(buf, "my:") != NULL)
		{
			strtok(buf, ":");
			char *tmp = strtok(NULL, ":");
			if (strstr(tmp, ".bmp") != NULL)
			{
				show_one_bmp(tmp, 2, 1);
				offset_y += 100;
			}
			else
			{
				int len = strlen(tmp);
				offset_x = 750 - len*15;
				Display_characterXX(offset_x,  70+offset_y, len+1, tmp, 2, 0x00000000); 
				offset_y += 30;
			}
		}
		else
		{
			if (strstr(buf, ".bmp") != NULL)
			{
				show_one_bmp(buf, 2, 2);
				offset_y += 100;
			}
			else
			{
				Display_characterXX(310,  70+offset_y, strlen(buf)+1, buf, 2, 0x00000000); 
				offset_y += 30;
			}
			
		}
	}
	
	fclose(fp);
	return 0;
}


//获取IP
char *sys_get_chat_member_ip(void)
{
	char *ip = calloc(1, 16);
	bzero(ip, 16);
	//判断ip位置
	int num = c_flag - 2;
		
	FILE *fp = fopen("./msg.txt", "r");
	if (fp == NULL)
	{
		perror("fopen failed sys ip");
		return NULL;
	}
	
	while(num--)
	{
		bzero(ip, 16);
		fscanf(fp, "%s", ip);
		
		if (strlen(ip) == 0)
		{
			printf("no obiect\n");
			fclose(fp);
			return NULL;
		}
	}
	sendip = ip;
	
	
	fclose(fp);
	return ip;
}
int sys_clean_msg_txt_msg(void)
{
	FILE *fp = fopen("./msg.txt", "w");
	if (fp == NULL)
	{
		perror("sys fopen failed");
		return -1;
	}
	fclose(fp);
	return 0;
}
//好友显示
int sys_reflush_member_to_lcd(char *_ip)
{
	FILE *fp = fopen("./msg.txt", "r");
	if (fp == NULL)
	{
		perror("sys fopen failed");
		return -1;
	}
	
	char ip_buf[16];

	int offset = 0;
	while(1)
	{
		if (feof(fp))
			break;
		
		bzero(ip_buf, 16);
		fscanf(fp, "%s", ip_buf);
		 if (strlen(ip_buf) < 5)
			 break;
#ifdef DEBUG
	printf("从msg.txt文件读出的好友信息:%s\n", ip_buf);
#endif
		show_one_bmp("./login/head.bmp", 4, offset/35);
		//打印到LCD屏上
		Display_characterXX(47,  75+offset, strlen(ip_buf)+1, ip_buf, 2, 0x00000000); 
		if ( _ip != NULL && !strcmp(_ip, ip_buf))
		{
#ifdef DEBUG
	printf("找到ip: %s\n", ip_buf);
#endif
			Display_characterXX(275,  75+offset, 2, "*", 2, 0x00ff0000); 
		}
			
		
		offset += 35;
		if (offset > 300)
			break;
	}
	
	fclose(fp);
	return 0;
}

//主界面显示账号和密码
//0 账号
//1 密码
int show_account_passwd_font(char *_str, int _flag)
{
	int len = strlen(_str);
	if (len == 0)
		return 0;
	
	if (_flag == 0)
		Display_characterXX(270,  115, len+1, _str, 2, 0x00000000); 
	else
	{
		char buf[len];
		for (int i=0; i<len; i++)
			buf[i] = '*';
		
		Display_characterXX(270,  205, len+1, buf, 2, 0x00000000); 
	}
		
		
}

//获取键盘输入字符
int get_keyboard_string(char *_str)
{
	//按键显示
	clean();
	show_one_bmp("./open/keyboard.bmp", 1, 0);
	
	bzero(_str, strlen(_str));
	while(1)
	{
		if (k_flag == 6)			//退出
			break;
		else if (c_flag == -2)
		{
			c_flag = 2;
			break;
		}
		else if (k_val == '+')                      
		{
			bzero(_str, strlen(_str));				//清空字符串
			clean_keyboard_font();				    //清空界面字体
			k_val = '#';							//清空标志位
		}	
		else if (k_val == '-')						
		{
			
			sys_clean_keyboard_font(_str);  	//清除原来字体
			back_one_bytes(_str);               //后退一位
			show_keyboard_menu_font(_str);      //刷新界面
			k_val = '#';						//清空标志位
		}	
		else if (k_val == '@' && bmppack_flag == true)
		{
			int n = sys_keyboard_select_bmp();
			k_val = '#';
			bmp_flag = -1;
			if (n == 0)
			{
#ifdef DEBUG
	printf("[%d] 取消表情包成功\n");
#endif
				//按键显示
				clean();
				show_one_bmp("./open/keyboard.bmp", 1, 0);
				c_flag = 2;
				k_flag = 2;
				continue;
			}
				
			
			//返回图片名
			sys_get_bmp_path(_str, n);
			
			break;
		}
		else if (k_val != '#')									
		{
			sys_clean_keyboard_font(_str);		//清除原来字体
			insert_char_to_string(_str);        //添加字符
			show_keyboard_menu_font(_str);      //刷新界面
			k_val = '#';						//清空标志位
		}
		
		
	}
	k_val = '#';
	
	return 0;
}


int sys_get_bmp_path(char *_str, int _num)
{
	PIC *pos;
	list_for_each_entry(pos, &head->list, list) 
	{
		--_num;
		if (_num == 0)
		{
			// _str = pos->picpath;
			strcpy(_str, pos->picpath);
			break;
		}
	}
	
	return 0;
}

int sys_keyboard_select_bmp(void)
{
	int num = sys_show_keyboard_bmp_pic();
	while(1)
	{
		if (bmp_flag == 0)
			return 0;
		
		if (bmp_flag <= num && bmp_flag != -1)
			return bmp_flag;
	}
}

int sys_show_keyboard_bmp_pic(void)
{
	PIC *pos;
	int n = 0;
	list_for_each_entry(pos, &head->list, list) 
	{
		show_one_bmp(pos->picpath, 3, n);
		++n;
	}
#ifdef DEBUG
		printf("[%d] 表情包数目：%d\n", __LINE__, n);
#endif
	return n;
}

//清除原来字体
int sys_clean_keyboard_font(char *_str)
{
	int len = strlen(_str);
	if (len == 0)
		return 0;
	
	clean_keyboard_font();
	
	return 0;
}

//按键界面字体显示
int show_keyboard_menu_font(char *_str)
{
	int len = strlen(_str);
	if (len == 0)
		return 0;
	
	Display_characterXX(20,  20, len+1, _str, 2, 0x00000000); 
	
	return 0;
}

//添加字符
int insert_char_to_string(char *_str)
{
	int len = strlen(_str);
	if (len >= 12)
		return 0;
	_str[len] =  k_val;
	_str[len+1] = '\0';
	
	return 0;
}

//退后一位
int back_one_bytes(char *_str)
{
	int len = strlen(_str);
	if (len == 0)
		return 0;
	
	_str[len-1] = '\0';
	return 0;
}






