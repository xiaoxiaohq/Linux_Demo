#include "myhead.h"
#include "kernel_list.h"

typedef struct msg
{
	int m_fd;
	char m_ip[16];
	struct list_head list;
}MSG;

static struct list_head *head = NULL;

int send_member_msg(int _fd)
{
	char s_msg[1024];
	bzero(s_msg, 1024);
	
	struct list_head *pos;
	
	char buf[1024];
	int n = 0;
	list_for_each(pos, head)
	{
		MSG *s_tmp = list_entry(pos, MSG, list);
		if (s_tmp->m_fd != _fd)
		{
			if (n == 0)
				sprintf(s_msg, "%s:", s_tmp->m_ip);		//拼接格式%s:
			else
			{
				bzero(buf, 1024);
				sprintf(buf, "%s%s:", s_msg, s_tmp->m_ip);		//拼接格式%s:
				strcpy(s_msg, buf);
			}
			
			n = 1;
		}			
	}
	
	char s_buf[SIZE] = {0};
	strcpy(s_buf, "member\n");
	send(_fd, s_buf, SIZE, 0);	//发送应答信号
#ifdef DEBUG
		printf("发送成员信息： %s\n", s_msg);
#endif
	if (n == 0)
		send(_fd, "NULL\n", 1024, 0); //发送链表数据
	else
		send(_fd, s_msg, 1024, 0); //发送链表数据
	
	return 0;
}

MSG *ip_to_fd(char *ip)
{
	struct list_head *pos;

	list_for_each(pos, head)
	{
		MSG *tmp = list_entry(pos, MSG, list);
		if(!strcmp(ip, tmp->m_ip))
			return tmp;
	}
	
	return NULL;
}
// ip:消息
// ip:file:表情包
void send_msg(char *msg, MSG *mymsg)
{
	//判断文件发送给谁的
	char *s_ip = strtok(msg, ":");   //提取ip号
	
#ifdef DEBUG
	printf("[%d] 获取到的对象IP： %s\n", __LINE__, s_ip);
#endif

	MSG *new = ip_to_fd(s_ip);		 //获取fd
	if (new == NULL)
	{
		printf("ip failed\n");
		return;
	}
	
	char *s_msg = strtok(NULL, ":"); 	//提取消息主体
	
	char buf[SIZE];
	if (strstr(s_msg, "file") == NULL) //判断是否是消息
	{
		bzero(buf, SIZE);
		sprintf(buf, "%s:%s", mymsg->m_ip, s_msg);
		send(new->m_fd, buf, SIZE, 0); 
#ifdef DEBUG
	printf("[%d] send :%s\n", __LINE__, buf);
#endif
		return;
	}
	
	
#ifdef DEBUG
	printf("[%d] send 图片开始\n", __LINE__);
#endif	
	//转发图片
	bzero(buf, SIZE);
	s_msg = strtok(NULL, ":");
	
	sprintf(buf, "%s:file:%s", mymsg->m_ip, s_msg);
	send(new->m_fd, buf, SIZE, 0);   				 //先发文件名 file:xxxx.bmp
#ifdef DEBUG
	printf("[%d] send 图片名:%s\n", __LINE__, buf);
#endif	
	
	bzero(buf, SIZE);
	recv(mymsg->m_fd, buf, SIZE, 0);		//接收图片大小
	
#ifdef DEBUG
	printf("[%d] recv 图片大小 :%s\n", __LINE__, buf);
#endif

	char *tmp = strtok(buf, "\n");
	if (tmp == NULL)
		strcpy(tmp, buf);
	
	send(new->m_fd, tmp, SIZE, 0);  //发送图片大小
	
	int len = atoi(tmp); 			 //计算接收图片数据的大小
	
#ifdef DEBUG
	printf("[%d] send 图片大小 :%d\n", __LINE__, len);
#endif	
	char pbuf[len];
	
	// recv(mymsg->m_fd, pbuf, len, 0);	//接收图片数据
	// send(new->m_fd, pbuf, len, 0);      //发送图片数据
	
	int n = len;
	while(1)
	{
		int m = recv(mymsg->m_fd, pbuf, n, 0);	//接收图片数据
		send(new->m_fd, pbuf, m, 0);  //发送图片数据
		bzero(pbuf, len);
		
		n -= m;
		if (n == 0)
			break;
	}
	
	
#ifdef DEBUG
	printf("[%d] send 图片完成\n", __LINE__);
#endif	
	return;
}

void del_client(int quitor)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, head)
	{
		MSG *tmp = list_entry(pos, MSG, list);
		if(tmp->m_fd == quitor)
		{
			list_del(pos);
			free(tmp);
			break;
		}
	}
}

void *rotinue(void *arg)
{
	char buf[SIZE];
	MSG *new = (MSG *)arg;
	while(1)
	{
		bzero(buf, SIZE);
		recv(new->m_fd, buf, SIZE, 0);
		printf("recv: %s\n", buf);
		if (!strcmp(buf, "exit\n"))                    //客户端退出前发送exit给服务器
			break;		
		if (!strcmp(buf, "get\n"))
		{
			send_member_msg(new->m_fd);					//发送成员链表	
		}
		else	
			send_msg(buf, new);
	}
	del_client(new->m_fd);
	pthread_exit(NULL);
}

int show_list_msg()
{
	struct list_head *pos;
	list_for_each(pos, head)
	{
		MSG *s_tmp = list_entry(pos, MSG, list);
		printf("%s\n", s_tmp->m_ip);
	}
}

int main(int argc, char *argv[])
{
	head = malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(head);
	
	//创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in seraddr, cliaddr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	bzero(&seraddr, addrlen);
	
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(50001);
	// seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_addr.s_addr = inet_addr("192.168.24.95"); //INADDR_ANY
	
#ifdef DEBUG
	printf("ip : %s\n",  inet_ntoa(seraddr.sin_addr));
#endif

	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	
	//绑定自己IP和端口号
	bind(sockfd, (struct sockaddr *)&seraddr, addrlen);
	
	//设置最大同时监听数量
	listen(sockfd, 3);
	
	while(1)
	{
		bzero(&cliaddr, addrlen);
		//等待客户端连接
		int connectfd = accept(sockfd, (struct sockaddr *)&cliaddr, &addrlen);
		
		printf("new connect: %s\n", inet_ntoa(cliaddr.sin_addr));
		
		MSG *new = malloc(sizeof(MSG));
		INIT_LIST_HEAD(&new->list);
		
		new->m_fd = connectfd;
		char *tmp = inet_ntoa(cliaddr.sin_addr);
		strcpy(new->m_ip, tmp);
		
		list_add(&new->list, head);
#ifdef DEBUG		
		show_list_msg();
#endif
		pthread_t id;
		pthread_create(&id, NULL, rotinue, (void *)new);
	}
	

	
	close(sockfd);
	return 0;
}