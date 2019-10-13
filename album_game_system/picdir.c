#include "demo_head.h"

int DIR_callback(struct node *head, char *dirname)
{
	DIR *dp = opendir(dirname);
	//printf("name: %s\n", dirname);
	if (dp == NULL)
	{
		perror("opendir()");
		return -1;
	}

	while(1)
	{
		struct dirent *msg = readdir(dp);
		if (msg == NULL)
		{
			closedir(dp);
			break;
		}
		if (msg->d_name[0] == '.')
		{
			continue;
		}
		
		//进入下一级目录
		if (msg->d_type == DT_DIR)
		{
			char buf[1024] = {0};
			int len = strlen(dirname);
			if (dirname[len-1] == '/')
				sprintf(buf, "%s%s",dirname,msg->d_name);
			else 
				sprintf(buf, "%s/%s",dirname,msg->d_name);
			DIR_callback(head, buf);
		}	
		
		if (msg->d_type == DT_REG)
		{
			if (strstr(msg->d_name, ".bmp") || strstr(msg->d_name, ".jpg") || strstr(msg->d_name, ".jpeg"))
			{
				char picpath[1024] = {0};
				sprintf(picpath, "%s/%s",dirname, msg->d_name);
				Create_and_insert_node(head, picpath);
			}
		}	
		
	}
	return 0;	
}



