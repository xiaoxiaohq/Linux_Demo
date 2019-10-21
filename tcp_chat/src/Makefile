#指定编译器
CC = arm-linux-gcc

#查找本目录文件
C_FILE = $(wildcard *.c)

#O_FILE=$(patsubst %.c,%.o,$(C_FILE)) 
#目标生成文件
TAR = main

#头文件路径
HEADER_PATH = -I../include  
#库路径
LIB_PATH = -L../lib  
#库  
LIBS = -lpthread  -lfont

${TAR}:
	$(CC) $(C_FILE) -o ${TAR} $(HEADER_PATH) $(LIB_PATH) $(LIBS) -DDEBUG 
	
clean:
	rm ${TAR}  


#SRC = ${wildcard *.c}
#OBJ = ${patsubst %.c, %.o, $(SRC)}
#
#CFLAGS = -c -Wall
#LIBFLAGS = -shared -fPIC
#
#all:$(OBJ)
#
#$(OBJ):%.o:%.c
#	$(CC) $(CFLAGS) -o $@ $^
#
#install:
#	$(CC) $(LIBFLAGS) -o libvnet.so $(OBJ)
#	cp libvnet.so /lib -f
#	cp head4sock.h /usr/include -f
#
#clean:
#	$(RM) $(OBJ) libvnet.so .*.sw?
#
#.PHONY:all clean
