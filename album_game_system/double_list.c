#include "demo_head.h"


//创建一个节点
struct node *Create_node(char *filename)
{
	struct node *new = (struct node *)malloc(sizeof(struct node));
	stpcpy(new->data, filename);
	//new->data = filename;
	new->prev = NULL;
	new->next = NULL;
	
	return new;
}

//头插法
void Insert_node(struct node *head, struct node *new)
{
	//head->data++;
	if (head->next == NULL)
	{
		head->next = new;
		new->prev = head;
	}
	else
	{
		new->next = head->next;
		head->next = new;
		
		new->next->prev = new;
		new->prev = head;
	}
}

void Create_and_insert_node(struct node *head, char *filename)
{
	struct node * new = Create_node(filename);
	Insert_node(head, new);
}
/*
//尾插法
void Insert_node_tail(struct node *head, struct node *new)
{
	head->data++;
	struct node *tmp = head;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	
	tmp->next = new;
	new->prev = tmp;
}

//删除节点
void Delete_node_from_linklist(struct node *head, int data)
{
	struct node *tmp = head->next;
	
	while(tmp != NULL)
	{
		if (tmp->data == data && tmp->next != NULL)
		{
			tmp->prev->next = tmp->next;
			tmp->next->prev = tmp->prev;
			free(tmp);
			head->data--;
		}
		if (tmp->data == data && tmp->next == NULL)
		{
			tmp->prev->next = NULL;
			free(tmp);
			head->data--;
		}
		tmp = tmp->next;
	}
	
}

//修改节点
void Modify_node_from_linklist(struct node *head, int data, int mdata)
{
	struct node *tmp = head->next;
	
	while(tmp != NULL)
	{
		if (tmp->data == data)
		{
			printf("修改数据 : %d   %d \n", data, mdata);
			tmp->data = mdata;
		}
		tmp = tmp->next;
	}	
}
*/
struct node * Array_end_of_linklist(struct node *head)
{
	struct node *tmp = head;
	
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	
	return tmp;
}
/*
//显示所有节点信息
void Show_double_linklist(struct node *head)
{
	struct node *tmp = head;
	printf("该链表有 %d 个节点\n", head->data);
	printf("----------begin---------\n");
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
		printf("%d\n", tmp->data);	
	}
	printf("------------------------\n");
	
	while(tmp->prev != NULL)
	{
		printf("%d\n", tmp->data);
		tmp = tmp->prev;
	}
	printf("----------end-----------\n");
}

//释放所有节点
void Free_double_linklist(struct node *head)
{
	struct node *tmp = head;
	
	while(head != NULL)
	{
		head = head->next;
		free(tmp);
		tmp = head;	
	}
}
*/
//查找链表的中间节点
/*******************************/
//根据head->data 查找
/*
void Find_middle_node_form_linklist_1(struct node *head)
{
	struct node *tmp = head->next;
	int len = head->data / 2;
	
	while(len-- != 1)
	{
		tmp = tmp->next;
	}
	
	if (head->data % 2 == 0)//偶
	{
		printf("中间节点有两个 数据为： %d  %d\n", tmp->data, tmp->next->data);
	}
	else//奇
	{
		printf("中间节点 数据为： %d \n", tmp->next->data);
	}
}
//遍历查找中间节点
void Find_middle_node_form_linklist_2(struct node *head)
{
	struct node *tmp = head->next;
	int len = 0;
	while(tmp != NULL)
	{
		tmp = tmp->next;
		len++;
	}
	
	len /= 2;
	tmp = head->next;
	while(len-- != 1)
	{
		tmp = tmp->next;
	}
	
	if (head->data % 2 == 0)//偶
	{
		printf("中间节点有两个 数据为： %d  %d\n", tmp->data, tmp->next->data);
	}
	else//奇
	{
		printf("中间节点 数据为： %d \n", tmp->next->data);
	}
	
}
*/
/*****************************/

//int main()
//{
//	struct node *head = Create_node(0);
//	
//	struct node *new = Create_node(100);
//	Insert_node(head, new);
//	new = Create_node(200);
//	Insert_node(head, new);
//	new = Create_node(300);
//	Insert_node(head, new);
//	new = Create_node(400);
//	Insert_node(head, new);
//	new = Create_node(500);
//	Insert_node_tail(head, new);
//	Show_double_linklist(head);
//	
//	Find_middle_node_form_linklist_1(head);
//	new = Create_node(600);
//	Insert_node_tail(head, new);
//	Show_double_linklist(head);
//	Find_middle_node_form_linklist_2(head);
//	
//	
//	Free_double_linklist(head);
//	return 0;
//}