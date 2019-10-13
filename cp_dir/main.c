#include "thread_pool.h"
#include "copyfile.h"

thread_pool *pool = NULL;

//获取进度条变量
int dirlen = 0;
int dstlen = 0;

//类型控制开关
bool Fflag = false;
//类型控制变量
char *Ttype = NULL;

//获取文件总数量确定线程数
int ctl_thread = 0;

//tree 变量
int tree_dirnum = 0;
int tree_filenum = 0;

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3 && argc != 5)
	{
		printf("Usage1: %s <dirpath> \n", argv[0]);
		printf("Usage1: %s <src> <dst>\n", argv[0]);
		printf("Usage2: %s <src> <dst> -t  <type>\n", argv[0]);
		return -1;
	}
	if (argc == 2)
	{
		printf("\033[1m\033[42;34m %s \033[0m\n", argv[1]);
		//printf("%s \n", argv[1]);
		char *prefix = "|";
		tree_show_dir(argv[1], prefix, NULL);
		printf("\n%d directories, %d files\n", tree_dirnum, tree_filenum);
		return 0;
	}
	
	if (argc == 5 && strcmp(argv[3], "-t"))
	{
		printf("Usage2: %s <src> <dst> -t  <type>\n", argv[0]);
		return -1;
	}
	
	
	//类型控制变量，初始化
	if (argc == 5 && !strcmp(argv[3], "-t"))
	{
		Ttype = argv[4];
		Fflag = true;
		printf("%s\n", Ttype);
	}
	
	//申请线程池
	pool = malloc(sizeof(thread_pool));
	if (pool == NULL)
	{
		perror("pool malloc failed");
		return -1;
	}
	
	//初始化线程池
	init_pool(pool, 5);
	
	printf("解析中\n");
	//遍历需要操作的目录
	//计算总目录大小
	//获取文件数量
	char *srcdir = argv[1];

	get_srcdir_reclen(srcdir, 0);
	if (dirlen == 0)
	{
		printf("no files!!!\n");
		destroy_pool(pool);
		return -1;
	}

	//当文件数目过多时，增加线程数量
	if (ctl_thread >= 30)
		add_thread(pool, 2);

	if (ctl_thread >= 50)
		add_thread(pool, 3);

	//先创建一个目录
	cp_first_dirname(argv[1], argv[2]);

	char *dstdir = argv[2];
	//创建一个线程来观察进度
	pthread_t id;
	pthread_create(&id, NULL, show_schedule, (void *)dstdir);

	//拷贝目录函数

	opendir_browse_file_add_task(argv[1], argv[2]);
	
	// 销毁线程次并回收线程
	destroy_pool(pool);
	//回收线程
	pthread_join(id, NULL);
	
	return 0;
}
