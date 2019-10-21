#include "demo_head.h"

static int sockfd;
static char *myip = NULL;  	//自己的IP
static char *recvip = NULL; //接收信息的ip
char *sendip = NULL; //你要发送信息的IP

bool recv_warn = false;  //当你没有连接对面，对面给你发消息时

int tcp_client(void)
{
	//创建套接字
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("sockfd failed");
		return -1;
	}

	struct sockaddr_in bindaddr;
	socklen_t addrlen = sizeof(bindaddr);
	
	bzero(&bindaddr, addrlen);
	bindaddr.sin_family = AF_INET;
	bindaddr.sin_port = htons(50001);
	bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	myip = calloc(1, 16);
	bzero(myip, 16);
	recvip = calloc(1, 16);
	bzero(recvip, 16);
	sendip = calloc(1, 16);
	bzero(sendip, 16);
	
	strcpy(myip,  inet_ntoa(bindaddr.sin_addr));
	// myip = inet_ntoa(bindaddr.sin_addr);

	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	//绑定自己的IP和端口
	int ret = bind(sockfd, (struct sockaddr *)&bindaddr, addrlen);
	if (ret == -1)
	{
		perror("bind failed");
		return -1;
	}
	struct sockaddr_in seraddr;
	bzero(&seraddr, addrlen);
	
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(50001);
	seraddr.sin_addr.s_addr = inet_addr("192.168.24.95");
	
	//连接服务器
	ret = connect(sockfd, (struct sockaddr *)&seraddr, addrlen);
	if (ret == -1)
	{
		perror("connect failed \n");
		return -1;
	}
	
	return 0;
}

int tcp_close(void)
{
#ifdef DEBUG
	printf("myip \n");
#endif	
	free(myip);
#ifdef DEBUG
	printf("recvip \n");
#endif	
	free(recvip);
#ifdef DEBUG
	printf("sendip \n");
#endif	
	free(sendip);
	send(sockfd, "exit\n", SIZE, 0);
	
	close(sockfd);
	return 0;
}


int tcp_refulsh_member_list(void)
{
	
#ifdef DEBUG
	printf("发送 get\n");
#endif

	//发送获取指令
	send(sockfd, "get\n", SIZE, 0);
	
	return 0;
}

//将发送的成员提取出来
int tcp_msg_to_file(char *_msg)
{
	FILE *fp = fopen("./msg.txt", "w+");
	if (fp == NULL)
	{
		perror("fopen failed");
		return -1;
	}
	
	if (!strcmp(_msg, "NULL\n"))
	{
#ifdef DEBUG
	printf("无好友在线\n");
#endif 
		fclose(fp);
		return 0;
	}
	char *tmp = strtok(_msg, ":");
	while(tmp != NULL)
	{
		fprintf(fp, "%s\n", tmp);
#ifdef DEBUG
	printf("写入msg.txt:%s\n", tmp);
#endif
		tmp = strtok(NULL, ":");	
	}
	
	fclose(fp);
	return 0;
}

//客户端接收线程
void *tcp_cli_recv_msg(void *arg)
{
	char t_buf[SIZE];
	char *t_tmp = NULL;
	while(1)
	{
		bzero(t_buf, SIZE);
		recv(sockfd, t_buf, SIZE, 0); //接收数据
#ifdef DEBUG
	printf("from server : %s\n", t_buf);
#endif
		if (!strcmp(t_buf, "member\n"))  //接收成员信息
		{
#ifdef DEBUG
	printf("recv member\n");
#endif

			char t_msg[1024] = {0};
			
			recv(sockfd, t_msg, 1024, 0); //接收成员信息
#ifdef DEBUG
	printf("recv ip :%s\n", t_msg);
#endif
			//将成员数据放到文件里面
			tcp_msg_to_file(t_msg);
			
			
		}
		else if (strstr(t_buf, "file:") != NULL) //接收图片    ip:file:xxxx
		{
			t_tmp = strtok(t_buf, ":/");
			strcpy(recvip, t_tmp);   //获取对方IP
			// recvip = t_tmp;				
#ifdef DEBUG
	printf("[%d] recv IP： %s\n", __LINE__, t_tmp);
#endif	
			char *tmp = NULL;       
			while(t_tmp != NULL)
			{
				tmp = t_tmp;                          //xxxx.bmp
				t_tmp = strtok(NULL, ":/");  //获取
			}
			
			t_tmp = strtok(tmp, "\n");
			if (t_tmp == NULL)
				t_tmp = tmp;
#ifdef DEBUG
	printf("[%d] 解析出文件名： %s\n", __LINE__, t_tmp);
#endif				
			//拼接自己的路径  ./recvpic
			char buf[512] = {0};
			sprintf(buf, "./recvpic/%s", t_tmp);
#ifdef DEBUG
	printf("[%d] 文件保存路径： %s\n", __LINE__, buf);
#endif				
			int fd = open(buf, O_CREAT|O_TRUNC|O_RDWR, 0666);
			if (fd == -1)
			{
				perror("open failed recv");
				continue;
			}
			
			//保存到本地文本
			tcp_add_msg_to_txt(buf);    
			
			bzero(t_buf, SIZE);
			recv(sockfd, t_buf, SIZE, 0);   //接收大小
#ifdef DEBUG
	printf("[%d] recv 文件大小： %s\n", __LINE__, t_buf);
#endif				
			tmp = strtok(t_buf, "\n");
			if (tmp == NULL)
				strcpy(tmp, t_buf);
			
			int piclen = atoi(tmp);
			
			char pbuf[piclen];
			
			// recv(sockfd, pbuf, piclen, 0);  //接收图片信息
			// write(fd, pbuf, piclen);     //写入本地
			
			int m = piclen;
			while(1)
			{
				int n = recv(sockfd, pbuf, m, 0);  //接收图片信息
			
				write(fd, pbuf, n);     //写入本地
				bzero(pbuf, piclen);
				m -= n;
				if (m == 0)
					break;
			}
#ifdef DEBUG
	printf("[%d] 图片接收完成\n", __LINE__);
#endif
			close(fd);
			
			recv_warn = true;
		}
		else									//普通消息
		{
			char *t_tmp = strtok(t_buf, ":");
			strcpy(recvip, t_tmp);   //获取对方IP
			t_tmp = strtok(NULL, ":");
			
			char *tt_tmp = strtok(t_tmp, "\n");
			if (tt_tmp == NULL)
				tt_tmp = t_tmp;
			//保存到本地文本
			tcp_add_msg_to_txt(tt_tmp);
#ifdef DEBUG
	printf("[%d] 保存到本地的信息:%s\n", __LINE__, tt_tmp);
#endif
			recv_warn = true;
		}		
	}
}

//将要发送对象的IP进行拼接
int tcp_send_msg_to_server(char *_msg)
{
	char buf[SIZE];
	bzero(buf, SIZE);
	if (strstr(_msg, ".bmp") != NULL) //图片
	{
		sprintf(buf, "%s:file:%s", sendip, _msg);
		send(sockfd, buf, SIZE, 0);
#ifdef DEBUG
			printf("[%d] 发送图片名：%s\n", __LINE__, buf);
#endif
		struct stat myst;
		bzero(&myst, sizeof(myst));
		stat(_msg, &myst);
		
		bzero(buf, SIZE);
		sprintf(buf, "%ld", myst.st_size);  //发送数据大小
		send(sockfd, buf, SIZE, 0);
		
#ifdef DEBUG
			printf("[%d] 发送图片大小：%s\n", __LINE__, buf);
#endif	
	
		int fd = open(_msg, O_RDONLY);
		if (fd == -1)
		{
			perror("open failed");
			return -1;
		}
		char rbuf[myst.st_size];
		
		// read(fd, rbuf, myst.st_size);
		// send(sockfd, rbuf, myst.st_size, 0); //发送图片信息
		
		int m = myst.st_size;
		while(1)
		{
			int n = read(fd, rbuf, myst.st_size);
			send(sockfd, rbuf, n, 0); //发送图片信息
			bzero(rbuf, myst.st_size);
			
			m -= n;
			if (m == 0)
				break;
		}
#ifdef DEBUG
			printf("[%d] 图片传输完成\n", __LINE__);
#endif
		
		close(fd);
	}	
	else //消息
	{
		sprintf(buf, "%s:%s", sendip, _msg);
#ifdef DEBUG
	printf("[%d] 对象发送的数据: %s\n", __LINE__, buf);
#endif
		send(sockfd, buf, strlen(buf), 0);  //发消息
	}
}

//清空本地日记
int tcp_clean_msg_txt(void)
{
	FILE *fp = fopen("./syslog.txt", "w+");
	if (fp == NULL)
	{
		perror("clean fopen failed");
		return -1;
	}
	fclose(fp);
	return 0;
}

int tcp_add_msg_to_txt(char *_msg)
{
	FILE *fp = fopen("./syslog.txt", "a");
	if (fp == NULL)
	{
		perror("syslog.txt fopen failed");
		return -1;
	}
	
	fprintf(fp, "%s\n", _msg);
	
	fclose(fp);
	
	return 0;
}
