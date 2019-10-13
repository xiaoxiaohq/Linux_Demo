#include "thread_pool.h"

/*************************************************
*	函数名: mutex_handler
*	函数功能：线程被打断时，调用该函数，避免出现死锁
*	函数参数：
*			arg  传参  传锁
*	函数返回值：NULL
**************************************************/
void mutex_handler(void *arg)
{
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

/*************************************************
*	函数名: init_pool
*	函数功能：初始化线程池
*	函数参数：
*			pool  线程池
*			threads_num  线程数量
*	函数返回值：
*			false 失败
*			true  成功
**************************************************/
bool init_pool(thread_pool *pool, unsigned int threads_num)
{
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->cond, NULL);
	
	pool->shutdown = false;
	pool->task_list = malloc(sizeof(struct task));
	pool->tids = malloc(sizeof(pthread_t)*MAX_ACTIVE_THREADS);
	
	if (pool->task_list == NULL || pool->tids == NULL)
	{
		perror("malloc memory error");
		return false;
	}
	
	pool->task_list->next = NULL;
	
	pool->waiting_tasks = 0;
	pool->active_threads = threads_num;
	
	for (int i=0; i<pool->active_threads; i++)
	{
		if (pthread_create(&((pool->tids)[i]), NULL, routine, (void *)pool) != 0)
		{
			perror("pthread_create error");
			return false;
		}
	}
	
	return true;
}
/*************************************************
*	函数名: add_task
*	函数功能：投放任务
*	函数参数：
*			pool  线程池
*			task  任务函数
*			arg   任务参数
*	函数返回值：
*			false 失败
*			true  成功
**************************************************/
bool add_task(thread_pool *pool, void *(*task)(void *arg), void *arg)
{
	//创建任务节点
	struct task *new_task = malloc(sizeof(struct task));
	if (new_task == NULL)
	{
		perror("malloc failed");
		return false;
	}
	
	new_task->task = task;
	new_task->arg = arg;
	new_task->next = NULL;

	//加锁，避免在加任务时，有线程在选任务
	pthread_mutex_lock(&pool->lock);
	//任务过多，不添加
	if (pool->waiting_tasks >= MAX_WAITING_TASKS)
	{
		pthread_mutex_unlock(&pool->lock);
		fprintf(stderr, "too many tasks\n");
		free(new_task);
		
		return false;
	}
	
	//在任务链表尾添加任务节点
	struct task *tmp = pool->task_list;
	while(tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_task;
	
	//任务数目也要增加
	pool->waiting_tasks++;
	
	//增加完成解锁
	pthread_mutex_unlock(&pool->lock);
	
	//唤醒一个线程执行任务
	pthread_cond_signal(&pool->cond);
	
	return true;
}

/*************************************************
*	函数名: add_thread
*	函数功能：添加线程数量
*	函数参数：
*			pool  线程池
*			add_threads_num  增加线程数量
*	函数返回值：
*			-1 失败 
*			>=0 成功返回增加线程数量
**************************************************/
int add_thread(thread_pool *pool, unsigned int add_threads_num)
{
	//数量为0，直接退出
	if (add_threads_num == 0)
		return 0;
	
	unsigned int total_threads = pool->active_threads + add_threads_num;
	
	//接着原线程id结构体继续创建线程
	int i, actual_incr = 0;
	for (i=pool->active_threads; i<total_threads && i<MAX_ACTIVE_THREADS; i++)
	{
		if (pthread_create(&((pool->tids)[i]), NULL, routine, (void *)pool) != 0)
		{
			perror("pthread_create failed");
			if (actual_incr == 0)
				return -1;
			break;
		}
		actual_incr++;
	}
	
	//创建完要修改线程数量
	pool->active_threads += actual_incr;
	return actual_incr;
}

/*************************************************
*	函数名: cancel_thread
*	函数功能：取消线程
*	函数参数：
*			pool  线程池
*			rm_threads_num  取消线程数量
*	函数返回值：
*			-1   失败
*			>=0  成功返回线程总数目
**************************************************/
int cancel_thread(thread_pool *pool, unsigned int rm_threads_num)
{
	//取消线程数为0，直接返回当前线程数目
	if (rm_threads_num == 0)
		return pool->active_threads;
	
	unsigned int remain_threads = pool->active_threads - rm_threads_num;
	//线程数目至少为1
	remain_threads = remain_threads > 0 ? remain_threads : 1;
	
	//取消线程
	//碰到线程无法取消，则退出
	int i;
	for (i=pool->active_threads-1; i>remain_threads-1; i--)
	{
		errno = pthread_cancel(pool->tids[i]);
		if (errno != 0)
			break;
	}
	
	if (i == pool->active_threads)
		return -1;
	else
	{
		pool->active_threads = i+1;
		return pool->active_threads;
	}
}

/*************************************************
*	函数名: destroy_pool
*	函数功能：销毁线程池
*	函数参数：
*			pool  线程池
*	函数返回值：
*			false 失败
*			true  成功
**************************************************/
bool destroy_pool(thread_pool *pool)
{
	//将退出标志位打开，并且广播所有线程，退出
	pool->shutdown = true;
	pthread_cond_broadcast(&pool->cond);
	
	//回收线程
	for (int i=0; i<pool->active_threads; i++)
	{
		errno = pthread_join(pool->tids[i], NULL);
		if (errno != 0)
			printf("join tids[%d] error: %s\n", i, strerror(errno));
		//else
		//	printf("[%u] is join \n", (unsigned int)pool->tids[i]);
	}   //
	
	//释放所有空间
	free(pool->task_list);
	free(pool->tids);
	free(pool);
	
	return true;
}

/*************************************************
*	函数名: routine
*	函数功能：所有线程调用函数
*	函数参数：arg  参数		
*	函数返回值：NULL
**************************************************/
void *routine(void *arg)
{
	thread_pool *pool = (thread_pool *)arg;
	struct task *p;
	
	while(1)
	{
		//入栈，防止死锁出现
		pthread_cleanup_push(mutex_handler, (void *)&pool->lock);
		//选任务前，先加锁
		pthread_mutex_lock(&pool->lock);
		
		//无任务时，阻塞
		while(pool->waiting_tasks == 0 && !pool->shutdown)
		{
			pthread_cond_wait(&pool->cond, &pool->lock);
		}
		
		//退出信号，且无任务
		if (pool->waiting_tasks == 0 && pool->shutdown)
		{
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL);
		}
		
		//取头任务，并且将头结点往后移动
		p = pool->task_list->next;
		pool->task_list->next = p->next;
		
		//取完任务，总任务标记需要减少
		pool->waiting_tasks--;
		
		//取完解锁
		pthread_mutex_unlock(&pool->lock);
		pthread_cleanup_pop(0);
		
		//执行任务，并且在此期间禁止响应取消请求
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		(p->task)(p->arg);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		
		//任务执行完释放任务空间
		free(p);
	}
	
	pthread_exit(NULL);
}

