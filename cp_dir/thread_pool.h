#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_WAITING_TASKS 1000
#define MAX_ACTIVE_THREADS 20

//任务节点
struct task 
{
	void *(*task)(void *); 		//任务函数
	void *arg;					//任务参数
	
	struct task *next;			//指向下一个任务节点
};

//线程池
typedef struct thread_pool
{
	pthread_mutex_t lock;    		//互斥锁
	pthread_cond_t cond;			//条件变量
	struct task *task_list;			//任务链表
	
	pthread_t *tids;				//所有线程id号
	
	unsigned int waiting_tasks;		//等待任务数目
	unsigned int active_threads;	//活跃线程数目
	
	bool shutdown;					//退出标记
}thread_pool;

//线程池初始化
bool init_pool(thread_pool *pool, unsigned int threads_num);

//投放任务
bool add_task(thread_pool *pool, void *(*task)(void *arg), void *arg);

//增加线程
int add_thread(thread_pool *pool, unsigned int add_threads_num);

//取消线程
int cancel_thread(thread_pool *pool, unsigned int rm_threads_num);

//销毁线程池
bool destroy_pool(thread_pool *pool);

//线程例程
void *routine(void *arg);

//响应取消请求后自动处理的例程，释放互斥锁
void mutex_handler(void *arg);

#endif
