#ifndef	__font__
#define	__font__

//��ʼ���ֿ� 
int Init_font(void);

//�����ֿ�
void  UnInit_Lcd(void);

//��ʾ����
int  Display_characterXX(unsigned int x,  //��ʾ�����X��
						unsigned int y, //��ʾ�����Y��
						unsigned int len, //����ĳ���
						unsigned char *string, //��Ҫ��ʾ�ĺ���
						int size, //���ֵĴ�С  1->8��
						unsigned long color); //���ֵ���ɫ

#endif