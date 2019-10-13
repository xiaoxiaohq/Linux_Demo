#include "demo_head.h"
#include <time.h>

void game_init()
{
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			buf_game[i][j] = 0;	

	data_refresh();
	gamescore = 0;
}

void refresh_window()
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			char tmp_buf[32] = {0};
			sprintf(tmp_buf, "./2048/%d.bmp", buf_game[i][j]);
			show_one_bmp(tmp_buf, 2, i+j*4+1);
		}
	}
}

int get_zero_pic_num()
{
	int num = 0;
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			if (buf_game[i][j] == 0)
				num++;
		}
	}
	return num;
}

int insert_pic_to_game(int num, int data)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			if (buf_game[i][j] == 0)
				num--;
			if (num == 0)
			{
				buf_game[i][j] = data;
				return 0;
			}	
		}
	}
	return 1;
}

void data_refresh()
{
	srand((unsigned int)time(NULL));
	int len = get_zero_pic_num();
	
	//添加一个 2 或 4
	int flag = rand()%2;
	int num = rand()%len;

	if (flag == 0)
	{
		insert_pic_to_game(num+1, 2);
	}
		
	if (flag == 1)
	{
		insert_pic_to_game(num+1, 4);
	}
}

//下
int down_move_pic()
{
	int score = 0;
	int i,j,k;
	for(k=0; k<4; k++)
	for (j=0; j<3; j++)
	{
		for(i=0; i<3-j; i++)
		{
			if(buf_game[k][3-i] == 0)
			{
				buf_game[k][3-i] = buf_game[k][3-i-1];
				buf_game[k][3-i-1] = 0;
			}
			else if(buf_game[k][3-i] == buf_game[k][3-i-1])
			{
				buf_game[k][3-i] *= 2 ;
				score += buf_game[k][3-i];
				buf_game[k][3-i-1] = 0;
			}
		}	
	}
	
	return score;
}

//上
int up_move_pic()
{
	int score = 0;
	int i,j,k;
	for(k=0; k<4; k++)
	for (j=0; j<3; j++)
	{
		for(i=0; i<3-j; i++)
		{
			if(buf_game[k][i] == 0)
			{
				buf_game[k][i] = buf_game[k][i+1];
				buf_game[k][i+1] = 0;
			}
			else if(buf_game[k][i] == buf_game[k][i+1])
			{
				buf_game[k][i] *= 2 ;
				score += buf_game[k][i];
				buf_game[k][i+1] = 0;
			}
		}	
	}
	
	return score;
}

//右
int right_move_pic()
{
	int score = 0;
	int i,j,k;
	for(k=0; k<4; k++)
	for (j=0; j<3; j++)
	{
		for(i=0; i<3-j; i++)
		{
			if(buf_game[3-i][k] == 0)
			{
				buf_game[3-i][k] = buf_game[3-i-1][k];
				buf_game[3-i-1][k] = 0;
			}
			else if(buf_game[3-i][k] == buf_game[3-i-1][k])
			{
				buf_game[3-i][k] *= 2 ;
				score += buf_game[3-i][k];
				buf_game[3-i-1][k] = 0;
			}
		}	
	}
	
	return score;
}

//左
int left_move_pic()
{
	int score = 0;
	int i,j,k;
	for(k=0; k<4; k++)
	for (j=0; j<3; j++)
	{
		for(i=0; i<3-j; i++)
		{
			if(buf_game[i][k] == 0)
			{
				buf_game[i][k] = buf_game[i+1][k];
				buf_game[i+1][k] = 0;
			}
			else if(buf_game[i][k] == buf_game[i+1][k])
			{
				buf_game[i][k] *= 2 ;
				score += buf_game[i][k];
				buf_game[i+1][k] = 0;
			}
		}	
	}
	
	return score;
}

//移动
int move_pic(int num)
{
	int score = 0;
	switch(num)
	{
		case 1:	
			score = right_move_pic(); 
		break;
		case 2:	
			score = left_move_pic();
		break;
		case 3:	
			score = up_move_pic();	
		break;
		case 4:	
			score = down_move_pic(); 
		break;
		default:
		break;	
	}
	
	return score;
}

int judge_game_move()
{
	int len = get_zero_pic_num();
	if (len > 0)
		return 0;
	
	//判断3X3
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			if (buf_game[i][j] == buf_game[i][j+1] || buf_game[i][j] == buf_game[i+1][j])
			{
				return 0;
			}
		}
	
	//判断剩下的
	for (int i=0; i<3; i++)
	{
		if (buf_game[i][3] == buf_game[i+1][3])
			return 0;
		if (buf_game[3][i] == buf_game[3][i+1])
			return 0;
	}
	
	return 1;
}

int judge_game_over(int tsfd)
{
	int ret = judge_game_move();
	if (ret)
	{
		show_one_bmp("./2048/full.bmp", 1, 0);
		while(1)
		{
			ret = ts_game_function(tsfd);
			if (ret == 0) //退出
			{
				game_init();
				return 2;
			}
			
			if (ret == 5) //重开
			{
				game_init();
				show_one_bmp("./2048/main.bmp", 1, 0);
				return 1;
			}		
		}
	}
	return 0;
}

void show_score()
{
	int num = gamescore;
	int i = 0;
	int flag = 0;
	char tmp_buf[32] = {0};
	
	i = num/100000;
	if (i)
	{
		sprintf(tmp_buf, "./score/%d.bmp", i);
		show_one_bmp(tmp_buf, 3, 1);
		flag = 1;
	}

	i = num/10000%10;
	if (i || flag)
	{
		sprintf(tmp_buf, "./score/%d.bmp", i);
		show_one_bmp(tmp_buf, 3, 2);
		flag = 1;
	}

	i = num/1000%10;
	if (i || flag)
	{
		sprintf(tmp_buf, "./score/%d.bmp", i);
		show_one_bmp(tmp_buf, 3, 3);
		flag = 1;
	}

	i = num/100%10;
	if (i || flag)
	{
		sprintf(tmp_buf, "./score/%d.bmp", i);
		show_one_bmp(tmp_buf, 3, 4);
		flag = 1;
	}

	i = num/10%10;
	if (i || flag)
	{
		sprintf(tmp_buf, "./score/%d.bmp", i);
		show_one_bmp(tmp_buf, 3, 5);
		flag = 1;
	}
	
	i = num%10;
	sprintf(tmp_buf, "./score/%d.bmp", i);
	show_one_bmp(tmp_buf, 3, 6);	
}

int game_function(int tsfd)
{
	//显示主界面
	show_one_bmp("./2048/main.bmp", 1, 0);
	//显示分数
	show_score();
	while(1)
	{
		//显示数据
		refresh_window();
		int ret = ts_game_function(tsfd);
		
		if (ret == 0) //退出
			break;
		if (ret == 5)	//重开
		{
			game_init();
			show_one_bmp("./2048/main.bmp", 1, 0);
			show_score();
			continue;
		}
		
		//移动图片
		gamescore += move_pic(ret);
		
		//显示分数
		show_score();
		
		int pret = judge_game_over(tsfd);
		if (pret == 2)
			return 0;
		
		//刷新数据
		data_refresh();
	}
}