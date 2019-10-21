#include "demo_head.h"

int DIR_callback(PIC *head, char *dirname)
{
	DIR *dp = opendir(dirname);
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
			char buf[512] = {0};
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
				char picpath[512] = {0};
				sprintf(picpath, "%s/%s",dirname, msg->d_name);
#ifdef DEBUG
	printf("[%d] 已有表情包 %s\n", __LINE__, picpath);
#endif
				PIC *new = malloc(sizeof(PIC));
				INIT_LIST_HEAD(&new->list);
				strcpy(new->picpath, picpath);
				// new->picpath = picpath;
				list_add_tail(&new->list, &head->list);
			}
		}	
		
	}
	return 0;	
}

int picdir_free_list(PIC *head)
{
	PIC *pos, *n;
	list_for_each_entry_safe(pos, n, &head->list, list)
	{
		list_del(&pos->list);
#ifdef DEBUG
	printf("free %s\n", pos->picpath);
#endif
		free(pos);
	}
}


