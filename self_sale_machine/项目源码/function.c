#include "demo_head.h"

char * ex_price = NULL;

//��ȡԪ�ַ�
void read_yuan_font()
{
	FILE *fp = fopen("font.txt", "r");
	if (fp == NULL)
	{
		printf("font.txt open error\n");
		return ;
	}
	fscanf(fp, "%s", ex_font);
	fclose(fp);
}
//�������
void insert_node_to_list_sort(Node *tmp)
{
	Node *pos;
	list_for_each_entry(pos, head, list)
	{
		if (atof(pos->price) > atof(tmp->price))
			break;
	}
	
	__list_add(&tmp->list, pos->list.prev, &pos->list);

}
//��ȡ�ı���Ϣ���ṹ��
void get_products_msg()
{
	FILE *fp = fopen("product.txt", "r");
	if (fp == NULL)
	{
		printf("open failes!!!!\n");
		return;
	}
	while(1)
	{
		Node *new = malloc(sizeof(Node));
		int ret = fscanf(fp,"%s %s %d %s\n", new->name, new->price, &new->num, new->filepath);
		if (ret == EOF)
		{
			free(new);
			break;
		}
		if (new->num == 0)
			new->num = 50;
		new->saleNum = 0;
		insert_node_to_list_sort(new);
	}

	fclose(fp);	
}

//��һ�� ��ʾ��Ʒ
struct list_head *show_product_pic(struct list_head *new)
{
	struct list_head *tmp = new;
	for(int i=0; i<6; i++)
	{

		if (i != 0 && tmp == new)
		{
			break;
		}
		if (i != 0 && tmp == head)
		{
			break;
		}
		if (tmp == head)
		{
			i--;
			tmp = tmp->next;
			continue;
		}
		Node * p = list_entry(tmp, Node, list);
		ex_price  = p->price;
		show_one_bmp(p->filepath, 2, i+1);
		
		tmp = tmp->next;
	}
	
	return tmp;
}

//��ǰ��
struct list_head *go_forward_six(struct list_head *new, int step)
{
	for(int i=0; i<step; i++)
		new = new->prev;
	return new;
}
//��ȡ��ǰ��Ʒ�ڵ�
struct list_head *locate_node(struct list_head *tmp, int step)
{
	for(int i=0; i<step; i++)
	{
		if (tmp->next == head)
			return NULL;
		tmp = tmp->next;
	}
	
	return tmp;
}

//��ȡ��ǰ��Ʒ��Ϣ
Node *get_product_form_list(struct list_head *tmp)
{
	Node * new = list_entry(tmp, Node, list);
	return new;
}

//��ʾ����Ʒ
void show_product_msg(Node *tmp)
{
	char path[1024] = {0};
	strcpy(path, tmp->filepath);

	int len = strlen(path);
	path[len-1-3] = 'b';
	path[len-1-2] = 'i';
	path[len-1-1] = 'g';
	path[len-1] = '.';
	path[len] = 'b';
	path[len+1] = 'm';
	path[len+2] = 'p';

	show_one_bmp(path, 0, 0);
}

//��ȡ��ʾ����
struct proXY * show_font_XY(int level)
{
	
	struct proXY *p = malloc(sizeof(struct proXY));
	if (level == 1) //����
	{
		p->x = 460;
		p->y = 120;
	}
	if (level == 2) //����
	{
		p->x = 480;
		p->y = 170;
	}
	if (level == 3) //����
	{
		p->x = 485;
		p->y = 200;
	}
	if (level == 4) //ѡ�в�Ʒ����
	{
		p->x = 460;
		p->y = 245;
	}
	return p;
}

//i to  a
char* itoa(int num,char* str,int radix)
{
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//������
	unsigned unum;//���Ҫת���������ľ���ֵ,ת�������������Ǹ���
	int i=0,j,k;//i����ָʾ�����ַ�����Ӧλ��ת��֮��i��ʵ�����ַ����ĳ��ȣ�ת����˳��������ģ��������������k����ָʾ����˳��Ŀ�ʼλ��;j����ָʾ����˳��ʱ�Ľ�����
 
	//��ȡҪת���������ľ���ֵ
	if(radix==10&&num<0)//Ҫת����ʮ�����������Ǹ���
	{
		unum=(unsigned)-num;//��num�ľ���ֵ����unum
		str[i++]='-';//���ַ�����ǰ������Ϊ'-'�ţ�����������1
	}
	else unum=(unsigned)num;//����numΪ����ֱ�Ӹ�ֵ��unum
 
	//ת�����֣�ע��ת�����������
	do
	{
		str[i++]=index[unum%(unsigned)radix];//ȡunum�����һλ��������Ϊstr��Ӧλ��ָʾ������1
		unum/=radix;//unumȥ�����һλ
 
	}while(unum);//ֱ��unumΪ0�˳�ѭ��
 
	str[i]='\0';//���ַ���������'\0'�ַ���c�����ַ�����'\0'������
 
	//��˳���������
	if(str[0]=='-') k=1;//����Ǹ��������Ų��õ������ӷ��ź��濪ʼ����
	else k=0;//���Ǹ�����ȫ����Ҫ����
 
	char temp;//��ʱ��������������ֵʱ�õ�
	for(j=k;j<=(i-1)/2;j++)//ͷβһһ�Գƽ�����i��ʵ�����ַ����ĳ��ȣ��������ֵ�ȳ�����1
	{
		temp=str[j];//ͷ����ֵ����ʱ����
		str[j]=str[i-1+k-j];//β����ֵ��ͷ��
		str[i-1+k-j]=temp;//����ʱ������ֵ(��ʵ����֮ǰ��ͷ��ֵ)����β��
	}
 
	return str;//����ת������ַ���
}

//��ʾ��Ʒ��Ϣfont
void show_font_product(Node *tmp, int _num)
{
	struct proXY * sxy = show_font_XY(1);
	Display_characterXX(sxy->x, sxy->y, strlen(tmp->name), tmp->name, 3, 0x00000000);
	
	sxy = show_font_XY(2);
	char buf[16] = {0};
	itoa(tmp->num,buf,10);
	Display_characterXX(sxy->x, sxy->y, strlen(buf)+1, buf, 2, 0x00000000);
	
	sxy = show_font_XY(3);
	Display_characterXX(sxy->x, sxy->y, strlen(tmp->price)+1, tmp->price, 2, 0x00000000);
	
	sxy = show_font_XY(4);
	bzero(buf, 16);
	itoa(_num,buf,10);
	Display_characterXX(sxy->x, sxy->y, strlen(buf)+1, buf, 3, 0x00ff0000);
}
//ˢ�²�Ʒ��Ϣfont
void show_font_product_refresh(Node *tmp, int _num)
{
	struct proXY *sxy = show_font_XY(2);
	char buf[16] = {0};
	itoa(tmp->num-1,buf,10);
	Display_characterXX(sxy->x, sxy->y, strlen(buf)+1, buf, 2, 0x0099D9EA);
	bzero(buf, 16);
	itoa(tmp->num+1,buf,10);
	Display_characterXX(sxy->x, sxy->y, strlen(buf)+1, buf, 2, 0x0099D9EA);
	bzero(buf, 16);
	itoa(tmp->num,buf,10);
	Display_characterXX(sxy->x, sxy->y, strlen(buf)+1, buf, 2, 0x00000000);
	
	sxy = show_font_XY(4);
	bzero(buf, 16);
	itoa(_num-1,buf,10);
	Display_characterXX(sxy->x, sxy->y, strlen(buf)+1, buf, 3, 0x0099D9EA);
	bzero(buf, 16);
	itoa(_num+1,buf,10);
	Display_characterXX(sxy->x, sxy->y, strlen(buf)+1, buf, 3, 0x0099D9EA);
	bzero(buf, 16);
	itoa(_num,buf,10);
	Display_characterXX(sxy->x, sxy->y, strlen(buf)+1, buf, 3, 0x00ff0000);
}
//����˵�
void free_shopping_account()
{
	FILE *sp = fopen("sp.txt", "w+");
	if (sp == NULL)
	{
		printf("sp.txt open error\n");
		return ;
	}
	fclose(sp);
}

//����˵��е�0
int free_shopping_account_zero()
{
	int flag = 0;
	FILE *sp = fopen("sp.txt", "r");
	FILE *sppp = fopen("sppp.txt", "w+");
	if (sp == NULL || sppp == NULL)
	{
		printf("sp.txt open error\n");
		return -1;
	}
	//��ȡ��ͬ��Ʒ
	char _name[1024], _price[128];
	int _num = 0;

	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		if (fscanf(sp, "%s %s %d", _name, _price, &_num) == EOF)
			break;
		if (_num != 0 && _name != NULL)
		{
			fprintf(sppp, "%s %s %d\n", _name, _price, _num);
			
		}
		
		if (_num == 0 && _name != NULL)
		{
			flag = 1;
		}
	}
	fclose(sppp);
	fclose(sp);
	
	sp = fopen("sp.txt", "w+");
	sppp = fopen("sppp.txt", "r");
	if (sp == NULL || sppp == NULL)
	{
		printf("sp.txt open error\n");
		return -1;
	}

	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		if (fscanf(sppp, "%s %s %d", _name, _price, &_num) == EOF)
			break;

		fprintf(sp, "%s %s %d\n", _name, _price, _num);
	}
	fclose(sppp);
	fclose(sp);
	
	if (flag == 0)
		return 0;
	if (flag == 1)
		return 1;
}
//���湺�ﳵ�ı�
int save_shopping_cart_txt(Node *tmp, int pnum)
{
	FILE *sp = fopen("sp.txt", "r+");
	if (sp == NULL)
	{
		printf("sp.txt open error\n");
		return -1;
	}
	rewind(sp);
	//��ȡ��ͬ��Ʒ
	char _name[1024], _price[128];
	int _num = 0;

	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		if (fscanf(sp, "%s %s %d", _name, _price, &_num) == EOF)
			break;
		if (!strcmp(_name, tmp->name))
		{
			char buf[1024] = {0};
			sprintf(buf, "%s %s %d", _name, _price, _num);
			long len = ftell(sp) - strlen(buf);
			fseek(sp, len, SEEK_SET);
			break;
		}
		
	}
	fprintf(sp, "%s %s %d\n", tmp->name, tmp->price, pnum);
	fclose(sp);
	return 0;
}

//��ӡ���ﳵ����
int show_shopping_kind_font()
{
	free_shopping_account_zero();
	
	FILE *ap = fopen("sp.txt", "r");
	
	char _name[1024];
	char _price[128];
	int _num = 0;
	int kind = 0;
	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		int ret = fscanf(ap, "%s %s %d", _name, _price, &_num);
		if (ret == EOF)
			break;
		kind++;
	}
	fclose(ap);
	if (kind > 0)
	{
		char buf[16] = {0};
		itoa(kind,buf,10);
		Display_characterXX(730, 345, strlen(buf)+1, buf, 3, 0x00ff0000);
	}
	return kind;
}

//������Ʒ
void browse_products_function(int tsfd)
{
	clean();
	show_one_bmp("./pic/browse.bmp", 0, 0); 
	//��ʾ��һ�Ž���
	struct list_head *tmp = head->next;					//ԭ��ָ���λ��
	struct list_head *new_tmp = show_product_pic(tmp); //����ָ���λ��
	//ͼƬ��ʾ��Ʒ
	int kind = 0;
	while(1)
	{
		kind = show_shopping_kind_font();
		//����ѡ��
		int ret = ts_browse_menu(tsfd);
		if (ret != 3 && ret != 0 && kind > 7)
		{
			show_one_bmp("./pic/full.bmp", 0, 0); 
			ret = 100;
		}
		
		if (ret == 0)	//�˳�
			break;
		if (ret == 1)	//��һ��
		{
			show_one_bmp("./pic/browse.bmp", 0, 0); 
			if(tmp->prev == head) //�ߵ�ͷ
			{
				new_tmp = show_product_pic(tmp);
			}
			else
			{
				tmp = go_forward_six(tmp, 6);
				new_tmp = show_product_pic(tmp);
			}			
		}
		if (ret == 2)	//��һ��
		{
			show_one_bmp("./pic/browse.bmp", 0, 0); 
			tmp = new_tmp;
			new_tmp = show_product_pic(tmp);
		}
		
		if (ret == 3) //���ﳵ
		{
			show_shopping_cart_function(tsfd);
			//�˳�ˢ�½���
			show_one_bmp("./pic/browse.bmp", 0, 0); 
			new_tmp = show_product_pic(tmp);
		}
		
		if (ret == 4 || ret == 5 || ret == 6 || ret == 7 || ret == 8 || ret == 9) //1--6
		{
			//��ȡ��Ʒ��Ϣ
			struct list_head *local_tmp = locate_node(tmp, ret-4); //0--5
			if (local_tmp == NULL)
				break;
			Node *local_pro = get_product_form_list(local_tmp);
			
			int amount = local_pro->num;
			int new_num = local_pro->saleNum;
			//��ʾ��Ʒ��Ϣ
			show_product_msg(local_pro);
			show_font_product(local_pro, local_pro->saleNum);
			while(1)
			{
				int pret = ts_product_show(tsfd);
				if (pret == 0 || pret == 3)	
					break;
				if (pret == 1) //��
				{
					(local_pro->saleNum == local_pro->num) ? local_pro->saleNum : (local_pro->saleNum++,local_pro->num--);
				}
				if (pret == 2) //��
				{
					(local_pro->saleNum == 0) ? 0 : (local_pro->saleNum--, local_pro->num++);
				}
				show_font_product_refresh(local_pro, local_pro->saleNum);
			}
			
			//�˳�ˢ�½���
			show_one_bmp("./pic/browse.bmp", 0, 0); 
			new_tmp = show_product_pic(tmp);
			
			//û��
			if (local_pro->saleNum == new_num)
			{
				local_pro->num = amount;
			}
			else
			{
				save_shopping_cart_txt(local_pro, local_pro->saleNum);					
			}
		}
		
		
	}
}

//����
void show_amount_money(int tsfd)
{
	clean();
	while(1)
	{
		show_one_bmp("./pic/amount.bmp", 0, 0);  //������ʾ
		int ret = ts_amount_function(tsfd);
		if (ret == 0)
			break;
	}
}

//�����˵���ʾλ��
void show_account_font(int offset, char *name, char *price, int num)
{
	Display_characterXX(85, 60+offset, strlen(name), name, 2, 0x00000000);
	Display_characterXX(300, 60+offset, strlen(price)+1, price, 2, 0x00000000);
	
	char buf[16] = {0};
	itoa(num, buf, 10);
	Display_characterXX(500, 60+offset, strlen(buf)+1, buf, 2, 0x00000000);
}

void show_account_font_manage(int offset)
{
	char buf[16] = {0};
	buf[0] = 'X';
	Display_characterXX(25, 53+offset, strlen(buf)+1, buf, 3, 0x00ff0000);
	
	bzero(buf, 16);
	buf[0] = '-';
	Display_characterXX(440, 52+offset, strlen(buf)+1, buf, 3, 0x00ff0000);
	bzero(buf, 16);
	buf[0] = '+'; 
	Display_characterXX(550, 51+offset, strlen(buf)+1, buf, 3, 0x00ff0000);
}

//�˵�ˢ��
void show_account_font_refresh(int offset, char *name, char *price, int num)
{
	char buf[16] = {0};
	itoa(num-1, buf, 10);
	Display_characterXX(500, 60+offset, strlen(buf)+1, buf, 2, 0x007092BE);
	itoa(num+1, buf, 10);
	Display_characterXX(500, 60+offset, strlen(buf)+1, buf, 2, 0x007092BE);
	itoa(num, buf, 10);
	Display_characterXX(500, 60+offset, strlen(buf)+1, buf, 2, 0x00000000);
}
//�˵�ˢ��
void show_shopping_product_account_refresh()
{
	FILE *ap = fopen("sp.txt", "r+");
	
	char _name[1024];
	char _price[128];
	int _num = 0;
	int offset = 0;
	float money = 0;
	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		_num = 0;
		int ret = fscanf(ap, "%s %s %d", _name, _price, &_num);
		if (ret == EOF)
			break;
		show_account_font_refresh(offset, _name,  _price, _num);
		offset += 50;
		money += (_num*atof(_price));
	}
	char buf[128] = {0};
	sprintf(buf, "%0.2f", money);
	Display_characterXX(600, 300, strlen(buf)+1, buf, 4, 0x00ff0000);
	fclose(ap);
}
//�˵�ˢ�¹������
void show_shopping_product_account_refresh_manage()
{
	FILE *ap = fopen("sp.txt", "r+");
	
	char _name[1024];
	char _price[128];
	int _num = 0;
	int offset = 0;

	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		_num = 0;
		int ret = fscanf(ap, "%s %s %d", _name, _price, &_num);
		if (ret == EOF)
			break;
		show_account_font_manage(offset);
		offset += 50;
	}
	fclose(ap);
}
//��ʾ�˵�
void show_shopping_product_account()
{
	FILE *ap = fopen("sp.txt", "r+");
	
	char _name[1024];
	char _price[128];
	int _num = 0;
	int offset = 0;
	float money = 0;
	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		_num = 0;
		int ret = fscanf(ap, "%s %s %d", _name, _price, &_num);
		if (ret == EOF)
			break;
		show_account_font(offset, _name,  _price, _num);
		offset += 50;
		money += (_num*atof(_price));
	}
	char buf[128] = {0};
	sprintf(buf, "%0.2f", money);
	Display_characterXX(600, 300, strlen(buf)+1, buf, 4, 0x00ff0000);
	fclose(ap);
}

//��ռ�����
void free_counter(char *_name)
{
	Node *tmp;
	list_for_each_entry(tmp, head, list)
	{
		if (!strcmp(tmp->name, _name))
		{
			tmp->saleNum = 0;
		}
	}
	
}
//�����˶���д�����ۼ�¼
void save_account_to_sale()
{
	FILE *ap = fopen("sp.txt", "r");
	FILE *src = fopen("sale.txt", "a");
	
	char _name[1024];
	char _price[128];
	int _num = 0;
	float money = 0;

	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		_num = 0;
		money = 0;
		
		int ret = fscanf(ap, "%s %s %d", _name, _price, &_num);
		if (ret == EOF)
			break;
		
		//��ռ�����
		free_counter(_name);
		
		money = atof(_price) * _num;
		fprintf(src, "%s %s %d %0.2f\n", _name, _price, _num, money);
	}
	fclose(ap);
	fclose(src);
}

//�����ṹ������Ϣ
Node *get_product_for_modify(char *_name)
{
	Node *pos;
	list_for_each_entry(pos, head, list)
	{
		if (!strcmp(_name, pos->name))
			return pos;
	}
	
	return NULL;
}
//���ﳵ�Ӽ�
int get_shopping_cart_add_del(int level)
{
	level += 1;
	int row = level / 3; //�޸ĵڼ���
	int num = level % 3; //0 -    1 +
	FILE *ap = fopen("sp.txt", "r+");
	rewind(ap);
	char _name[1024];
	char _price[128];
	int _num = 0;
	while(row--)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		_num = 0;
		
		int ret = fscanf(ap, "%s %s %d", _name, _price, &_num);
		if (ret == EOF)
			return -1;
	}
	
	//�ҵ�Ҫд��Ϣ

	char buf[1024] = {0};
	sprintf(buf, "%s %s %d", _name, _price, _num);
	long len = ftell(ap) - strlen(buf);
	fseek(ap, len, SEEK_SET);
	
	Node *tmp = get_product_for_modify(_name);
	
	//��д����
	if (num == 0)
	{
		(_num == 0) ? 0 : (_num--, tmp->num++, tmp->saleNum--);
		
	}		
	else if(num == 2)
	{
		tmp->num += tmp->saleNum;
		tmp->saleNum = 0;
		_num = 0;
	}
	else
	{
		(_num == tmp->num) ? tmp->num : (_num++, tmp->num--, tmp->saleNum++);	
	}
		
	fprintf(ap, "%s %s %d\n", _name, _price, _num);
	fclose(ap);
	
	return 0;
}
int manage_shopping_cart(int tsfd)
{
	clean_manage_font();
	while(1)
	{
		//�������
		show_shopping_product_account_refresh_manage();
		char manage[128] = "����";
		Display_characterXX(5, 10, strlen(manage)+1, manage, 2, 0x00ff0000);
		int ret = ts_manage_shopping_cart(tsfd);
		if (ret == 0) //�˳�
			break;
		if (ret == 1) //��ʾ�˵�
			break;
		
		//ˢ�½���
		get_shopping_cart_add_del(ret);	
		clean_money();
		show_shopping_product_account_refresh();
		
		if (free_shopping_account_zero() == 1)
		{
			show_one_bmp("./pic/shopping.bmp", 0, 0); 
			//��ʾ��Ʒ��Ϣ
			show_shopping_product_account();	
		}
	}
}
//���ﳵ
void show_shopping_cart_function(int tsfd)
{
	clean();
	show_one_bmp("./pic/shopping.bmp", 0, 0);  //������ʾ
	//��ʾ��Ʒ��Ϣ
	show_shopping_product_account();
	while(1)
	{
		char manage[128] = "����";
		Display_characterXX(5, 10, strlen(manage)+1, manage, 2, 0x00ff0000);
		int ret = ts_shopping_cart(tsfd);
		if (ret == 0)
			break;
		if (ret == 1)
		{
			//����
			show_amount_money(tsfd);
			//д�����ۼ�¼
			save_account_to_sale();
			//����˵�
			free_shopping_account();
			//��д��Ʒ�ı�
			retell_product_txt();
			break;
		}	
		if (ret == 2) //����
		{
			int pret = manage_shopping_cart(tsfd);
			//ˢ�¹��ﳵ����
			show_one_bmp("./pic/shopping.bmp", 0, 0);  //������ʾ
			show_shopping_product_account();
			clean_manage_font();
		}		
	}
}

//����������ʾλ��
void show_sale_font(int offset, char *name, char *price, int num, float money)
{
	Display_characterXX(25, 50+offset, strlen(name), name, 1, 0x00000000);
	Display_characterXX(150, 50+offset, strlen(price)+1, price, 1, 0x00000000);
	char buf[16] = {0};
	itoa(num, buf, 10);
	Display_characterXX(295, 50+offset, strlen(buf)+1, buf, 1, 0x00000000);
	bzero(buf, 16);
	sprintf(buf, "%0.2f", money);
	Display_characterXX(420, 50+offset, strlen(buf)+1, buf, 1, 0x00000000);
}

//������۱�
void free_sale_account()
{
	FILE *sr = fopen("sale.txt", "w+");
	fclose(sr);
}

//��ӡʱ��
void show_sale_account_time_font(int offset)
{
	char pbuf[1024] = {0};
	time_t t = time(NULL);
	struct tm *pt = localtime(&t);
	pt->tm_year = 2019 - 1900;
	strcpy(pbuf,  asctime(pt));
	Display_characterXX(510, 50+offset, strlen(pbuf),pbuf, 1, 0x00000000);
}

//�������ۼ�¼
int show_sale_font_to_lcd(FILE *fp)
{
	char _name[1024];
	char _price[128];
	int _num = 0;
	int offset = 0;
	float money = 0;
	while(1)
	{
		bzero(_name, 1024);
		bzero(_price, 128);
		_num = 0;
		money = 0;
		
		int ret = fscanf(fp, "%s %s %d %f", _name, _price, &_num, &money);
		if (ret == EOF)
		{
			if (offset == 0)
				rewind(fp);
			break;
		}
			
		show_sale_font(offset, _name,  _price, _num, money);
		
		//��ӡʱ��
		show_sale_account_time_font(offset);
		
		offset += 20;
		if (offset >= 420)
			break;	
	}
	return 0;
}

//���ۼ�¼
void sale_record_function(int tsfd)
{
	clean();
	show_one_bmp("./pic/sale.bmp", 0, 0);
	
	FILE *fp = fopen("sale.txt", "r");
	
	show_sale_font_to_lcd(fp);
	
	while(1)
	{
		int ret = ts_show_sale_account(tsfd);
		if (ret == 0)
			 break;
		if (ret == 1) //��һҳ
		{
			clean();
			show_one_bmp("./pic/sale.bmp", 0, 0);
			show_sale_font_to_lcd(fp);
		}
		if (ret == 2)
		{
			rewind(fp);
			clean();
			show_one_bmp("./pic/sale.bmp", 0, 0);
			show_sale_font_to_lcd(fp);
		}
	}
	
	fclose(fp);
}

//��д��Ʒ�ı�
int retell_product_txt()
{
	Node *pos;
	FILE *fp = fopen("product.txt", "w+");
	if (fp == NULL)
		return -1;
	
	list_for_each_entry(pos, head, list)
	{
		fprintf(fp, "%s %s %d %s\n", pos->name, pos->price, pos->num, pos->filepath);
	}
	
	fclose(fp);
	return 0;
}

//��������
int open_menu(int tsfd)
{
	while(1)
	{
		clean();
		show_one_bmp("./pic/open.bmp", 0, 0);
		int ret = ts_open_show(tsfd);
		if (ret == 1)
			break;
	}
	
	return 0;
}