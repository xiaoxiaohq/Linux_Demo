#指定编译器
CC = arm-linux-gcc

#查找本目录文件
C_FILE = $(wildcard *.c)

#O_FILE=$(patsubst %.c,%.o,$(C_FILE)) 
#目标生成文件
TAR = main

#头文件路径
HEADER_PATH = -I./jpeglib/include  -I./
#库路径
LIB_PATH = -L./jpeglib/lib  
#库  
LIBS = -ljpeg   

${TAR}:
	$(CC) $(C_FILE) -o ${TAR} $(HEADER_PATH) $(LIB_PATH) $(LIBS)
	
clean:
	rm ${TAR}  


