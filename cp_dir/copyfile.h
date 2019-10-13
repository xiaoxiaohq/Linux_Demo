#ifndef _COPYFILE_H
#define _COPYFILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define  SIZE  1024*1024
//传参结构体
typedef struct dirpath
{
	char srcpath[512];
	char dstpath[512];
}dirarg;

//创建第一个目录
bool cp_first_dirname(char *dirpath, char *dstpath);

//线程拷贝任务函数
void *thread_cp_task(void *arg);

//拷贝主函数，分配任务
bool opendir_browse_file_add_task(char *dirpath, char *dstpath);

void get_srcdir_reclen (char *dirpath, int flag);
void *show_schedule(void *arg);
bool specified_file_cp(const char *filename);
bool tree_show_dir(char *dirpath, char *prefix, char *tmpfix);
void show_dirlen(void);

#endif
