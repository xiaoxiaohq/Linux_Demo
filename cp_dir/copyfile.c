#include "copyfile.h"
#include "thread_pool.h"

extern thread_pool *pool;
extern char *Ttype;
extern bool Fflag;
extern int dirlen;
extern int dstlen;
extern int ctl_thread;
/**************************************************************
*	函数名：cp_first_dirname
*  	函数功能：找到第一个首目录，创建它，并且将它拼接到目标地址
*	函数参数：
*			dirpath 	操作目录地址
*			dstpath		目标目录地址
*	返回值： true  操作成功
*			 false 失败
***************************************************************/
bool cp_first_dirname(char *dirpath, char *dstpath)
{
	//计算操作目录长度
	int len = strlen(dirpath);
	if (dirpath[len-1] == '/')
		return true;
	//取出目录名
	int i=0;
	for (i=len-2; i>=0; i--)
	{
		if (dirpath[i] == '/')
			break;
	}
	char tmp[100] = {0};
	for (int j=i+1; j<len; j++)
		tmp[j-i-1] = dirpath[j];

	//计算目标目录长度
	len = strlen(dstpath);
	char *ptmp = dstpath;

	//拼接目录路径
	if (dstpath[len-1] == '/')
		sprintf(dstpath, "%s%s", ptmp, tmp);
	else if (dstpath[len-1] != '/')
		sprintf(dstpath, "%s/%s", ptmp, tmp);
	

	//创建第一级目录
	if (mkdir(dstpath, 0666) != 0)
	{
		if (errno != EEXIST)
		{
			perror("mkdir failed");
			return -1;
		}	
	}
		
	return true;
}

/**************************************************************
*	函数名：thread_cp_task
*  	函数功能：线程任务函数，任务执行拷贝
*	函数参数：
*			arg 	外部传入参数 	
*	返回值： NULL		 
***************************************************************/
void *thread_cp_task(void *arg)
{
	dirarg *dirp = (dirarg *)arg;

	//打开文件
	int src = open(dirp->srcpath, O_RDONLY);
	int dst = open(dirp->dstpath,O_CREAT|O_TRUNC|O_RDWR, 0644);
	
	if (src == -1 || dst == -1)
	{
		printf("%s open failed\n", dirp->srcpath);
		return NULL;
	}
	
	//拷贝文件
	char buf[SIZE];
	while(1)
	{
		bzero(buf, SIZE);
		int ret = read(src, buf, SIZE);
		if (ret == 0)
			break;
		
		write(dst, buf, ret);
	}
	
	//关闭文件
	close(src);
	close(dst);

	return NULL;
}

/**************************************************************
*	函数名：opendir_browse_file_add_task
*  	函数功能：浏览目录，投送任务
*	函数参数：
*			dirpath		操作目录地址
*			dstpath	    目标目录地址
*	返回值： false   失败		 	 
***************************************************************/
bool opendir_browse_file_add_task(char *dirpath, char *dstpath)
{
	//打开目录
	DIR *dp = opendir(dirpath);
	if (dp == NULL)
	{
		perror("dir open failed");
		return false;
	}
	
	struct dirent *ep = NULL;	
	while(1)
	{
		//遍历读目录
		ep = readdir(dp);
		//到目录尾退出
		if (ep == NULL)
			break;
		
		//目录文件
		if (ep->d_type == DT_DIR)
		{
			//排除. .. 目录
			if (ep->d_name[0] == '.') 
				continue;
		
			//拼接目录路径
			char tmp[512] = {0};
			sprintf(tmp, "%s/%s", dirpath, ep->d_name);
			
			char ptmp[512] = {0};
			sprintf(ptmp, "%s/%s", dstpath, ep->d_name);
			
			//新建子目录
			if (mkdir(ptmp, 0666) != 0)
			{
				if (errno != EEXIST)
				{
					perror("mkdir failed");
					return false;
				}	
			}
	
			//递归遍历目录
			opendir_browse_file_add_task(tmp, ptmp);
		}
		
		//普通文件
		if (ep->d_type == DT_REG)
		{
			//拼接文件路径
			char tmp[512] = {0};
			sprintf(tmp, "%s/%s", dirpath, ep->d_name);
			
			//判断是否满足下面的情况
			//1、拷贝所有文件
			//2、拷贝Ttype类型文件
			if ((Fflag == false) || (specified_file_cp(ep->d_name) && Fflag))
			{
				char ptmp[512] = {0};
				sprintf(ptmp, "%s/%s", dstpath, ep->d_name);
				
				dirarg *dirp = malloc(sizeof(dirarg));
				strcpy(dirp->srcpath, tmp);
				strcpy(dirp->dstpath, ptmp);
				
				//将文件放入线程池
				add_task(pool, thread_cp_task, (void *)dirp);
			}	
		}
	}
	//关闭目录
	closedir(dp);
}

/**************************************************************
*	函数名：get_srcdir_reclen
*  	函数功能：获取目录大小
*	函数参数：
*			dirpath		操作目录地址
*			flag		0   dirlen  获取操作目录大小
						1   dstlen  获取目标目录大小
*	返回值： 无		 	 
***************************************************************/
void get_srcdir_reclen (char *dirpath, int flag)
{
	//打开目录
	DIR *dp = opendir(dirpath);
	struct dirent *ep = NULL;
	
	while((ep = readdir(dp))!=NULL)
	{	
		//目录文件
		char tmp[512] = {0};
		if (ep->d_type == DT_DIR)
		{
			if (ep->d_name[0] == '.')
				continue;
		
			bzero(tmp, 512);
			int len = strlen(dirpath);
			if (dirpath[len-1] == '/')
				sprintf(tmp, "%s%s", dirpath, ep->d_name);
			else
				sprintf(tmp, "%s/%s", dirpath, ep->d_name);
			
			//递归
			get_srcdir_reclen(tmp, flag);
		}
		//普通文件
		else if (ep->d_type == DT_REG)
		{
			bzero(tmp, 512);
			int len = strlen(dirpath);
			if (dirpath[len-1] == '/')
				sprintf(tmp, "%s%s", dirpath, ep->d_name);
			else
				sprintf(tmp, "%s/%s", dirpath, ep->d_name);
			
			//不限定类型进入
			//限定类型，类型匹配正确进入
			
			if (Fflag == false || (specified_file_cp(ep->d_name) && Fflag == true))
			{
				struct stat sb;
				stat(tmp, &sb);
				
				if (flag == 0)
					dirlen += sb.st_size;
				else if (flag == 1)
					dstlen += sb.st_size;
				
				ctl_thread++; //计算文件数
			}
			
		}	
	}
	//关闭目录
	closedir(dp);
}
/**************************************************************
*	函数名：show_dirlen
*  	函数功能：显示操作目录大小
*	函数参数：void
*	返回值： void	 	 
***************************************************************/
void show_dirlen(void)
{
	float size = (float)dirlen/1024;
	if (size > 1000)
	{
		size /= 1024;
		printf("拷贝目录大小：%0.2f MB\n", size);
		return;
	}
	printf("拷贝目录大小：%0.2f KB\n", size);
}
/**************************************************************
*	函数名：show_schedule
*  	函数功能：进度条显示线程
*	函数参数：
*			arg		外部传参
*	返回值： NULL	 	 
***************************************************************/
void *show_schedule(void *arg)
{
	printf("解析完成\n");
	show_dirlen();
	while(1)
	{
		float perc = ((float)dstlen)/(float)dirlen*100;
		printf("\r拷贝进度 %0.2f %% ", perc);
		for (int i=1; i<=(int)(perc/5); i++)
			printf("*");
		fflush(stdout);
		
		if (dstlen >= dirlen)
			break;
		
		//加点延迟，防止一直在游览目录
		usleep(10000);	
		
		dstlen = 0;
		//获取目标目录大小
		get_srcdir_reclen((char *)arg, 1);	
	}
	printf("\n");
	if (dstlen > dirlen)
		printf("拷贝出错\n");
	pthread_exit(NULL);
}

/**************************************************************
*	函数名：specified_file_cp
*  	函数功能：判断文件类型是否与Ttype一致
*	函数参数：
*			filename	文件名
*	返回值： true   相同
*			 false  不同
***************************************************************/
bool specified_file_cp(const char *filename)
{
	int len = strlen(filename);
	char buf[10] = {0};

	int i=0;
	for(i=len-1; i>=0; i--)
	{
		if (filename[i] == '.')
			break;
	}	

	if (i == 0)
		return false;

	int j;
	for (j=i+1; j<len; j++)
		buf[j-i-1] = filename[j];
	buf[len-i-1] = '\0';
	
	//比较文件名
	if (strcmp(Ttype, buf))
		return false;
		
	return true;
}

/**************************************************************
*	函数名：judge_tail_file
*  	函数功能：判断是否是最后一个文件夹
*	函数参数：
*			name	 比较文件名
*			dirpath  操作目录
*	返回值： true   到尾
*			 false  没到
***************************************************************/
bool judge_tail_file(const char *name, const char *dirpath)
{
	//打开目录
	DIR *dp = opendir(dirpath);
	if (dp == NULL)
	{
		perror("opendir failed");
		return false;
	}
	struct dirent *ep =NULL;
	while(1)
	{
		//读目录文件
		ep = readdir(dp);
		//读到一样的退出
		if (!strcmp(ep->d_name, name))
			break;
		if (ep == NULL)
		{
			closedir(dp);
			return false;
		}		
	}
	
	//判断下一个是否到尾
	ep = readdir(dp);
	if (ep == NULL)
	{
		closedir(dp);
		return true;
	}
	closedir(dp);
	return false;
}

/**************************************************************
*	函数名：tree_show_dir
*  	函数功能：树状显示目录文件
*	函数参数：
*			dirpath	 操作目录地址
*			prefix   显示前缀
*			tmpfix   最后一个目录显示签字  （用一个flag也可以判断， 懒得改啦）
*	返回值：  false  出错
***************************************************************/

extern int tree_dirnum;
extern int tree_filenum;
//printf("\033[1m\033[42;36m HELLO_WORLD \033[0m\n");
// 字背景颜色范围: 40--49                   字颜色: 30—39             
//  40: 黑                           30: 黑                 
//  41: 红                           31: 红                
//  42: 绿                           32: 绿                
//  43: 黄                           33: 黄                
//  44: 蓝                           34: 蓝               
//  45: 紫                           35: 紫                 
//  46: 深绿                       36: 深绿                 
//  47: 白色                       37: 白色
// ANSI控制码:
//  QUOTE: 
//    \033[0m   关闭所有属性      
//    \033[1m   设置高亮度      
//    \033[4m   下划线      
//    \033[5m   闪烁      
//    \033[7m   反显      
//   \033[8m   消隐    
//   \033[30m   --   \033[37m   设置前景色     
//   \033[40m   --   \033[47m   设置背景色      
//   \033[nA   光标上移n行     
//   \03[nB   光标下移n行      
//   \033[nC   光标右移n行     
//   \033[nD   光标左移n行
//printf("\033[1m\033[42;34m %s \033[0m\n", argv[1]);  目录
//printf("\033[1m\033[40;32m %s \033[0m\n", argv[1]);  文件
bool tree_show_dir(char *dirpath, char *prefix, char *tmpfix)
{
	//打开目录
	DIR *dp = opendir(dirpath);
	if (dp == NULL)
	{
		perror("opendir failed");
		return false;
	}
	
	struct dirent *ep = NULL;
	while(1)
	{
		//读目录
		ep = readdir(dp);
		if (ep == NULL)
			break;
		
		//目录文件
		if (ep->d_type == DT_DIR)
		{
			if (ep->d_name[0] == '.')
				continue;
		
			//拼接目录路径
			char tmp[512] = {0};
			int len = strlen(dirpath);
			if (dirpath[len-1] == '/')
				sprintf(tmp, "%s%s", dirpath, ep->d_name);
			else
				sprintf(tmp, "%s/%s", dirpath, ep->d_name);
			
			//第一级目录的最后一个目录操作
			//要去掉最开始的|
			char ptmp[100] = {0};
			if (prefix == NULL)
			{
				sprintf(ptmp,"%s---",tmpfix);
				printf(" ");
				for (int i=1; i<strlen(ptmp); i++)
					printf("%c", ptmp[i]);
				printf("\033[1m\033[42;34m %s \033[0m\n", ep->d_name);
				// printf(" %s\n", ep->d_name);
			}
			//其它路径的目录拼接
			else
			{
				sprintf(ptmp,"%s---",prefix);
				printf("%s\033[1m\033[42;34m %s \033[0m\n", ptmp, ep->d_name);
				//printf("%s %s\n", ptmp, ep->d_name);
			}
				
			//判断是不是最后一个文件
			bzero(ptmp, 100);
			if (judge_tail_file(ep->d_name, dirpath))
			{
				//第一级目录的最后一个目录的子目录操作
				if (prefix == NULL)
				{
					int len = strlen(tmpfix);
					while(tmpfix[len-1] != '|')
						len--;
					
					tmpfix[len-1] = ' ';
				}
				//其它目录的子目录操作
				else
				{
					int len = strlen(prefix);
					while(prefix[len-1] != '|')
						len--;
					
					if (len-1 == 0)
					{
						tmpfix = prefix;
						prefix = NULL;
					}	
					else
						prefix[len-1] = ' ';
				}
			}

			//最后一个目录为第一级子目录的最后一个目录
			if (prefix == NULL)//最后一个目录操作
			{
				sprintf(ptmp, "%s    |", tmpfix);
				tree_show_dir(tmp, prefix, ptmp);
			}	
			else //未到最后一个目录
			{
				sprintf(ptmp, "%s    |", prefix);
				tree_show_dir(tmp, ptmp, NULL);
			}
			//计算目录数目
			tree_dirnum++;
		}
		
		//普通文件
		if (ep->d_type == DT_REG)
		{
			//第一级目录的子目录文件操作
			if (prefix == NULL)
			{
				printf(" ");
				for (int i=1; i<strlen(tmpfix); i++)
					printf("%c", tmpfix[i]);
				// printf("--- %s\n", ep->d_name);
				printf("--- \033[1m\033[32m%s \033[0m\n", ep->d_name);
			}
			//其它文件操作
			else
				//printf("%s--- %s\n", prefix, ep->d_name);
				printf("%s--- \033[1m\033[32m%s \033[0m\n", prefix, ep->d_name);
				
			//计算文件数目
			tree_filenum++;
		}	
	}
	closedir(dp);
}

