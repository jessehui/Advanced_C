#Designed for Samsung S5PV210

#define var
TARGET 	:= 	led.bin #最终目标
BUILD 	:=	led	

COBJS 	+= 	start.o
COBJS 	+= 	main.o
#COBJS 	+= 	delay.o 

ENV 	?= 	SD	#ENV未定义的话就定义为SD
SDTOOLS := 	./mk210

#工具集
CROSS_COMPILE := arm-linux-
CC 	:= $(CROSS_COMPILE)gcc
LD 	:= $(CROSS_COMPILE)ld	#链接器
OBJCOPY := $(CROSS_COMPILE)objcopy

#编译选项
CFLAGS 	+= -Wall 	#所有警告打开 warning 
CFLAGS 	+= -I./inc	#增加查找头文件的路径 当前目录下的inc目录中寻找头文件

#链接选项
ifeq($(ENV),RAM)
LDFLAGS += -Ttext=0x0	#链接代码段地址 如果从RAM启动,链接RAM基地址0
else
LDFLAGS += -Ttext=0x200000000 #SD卡启动基地址

#define way 操作方式
all:$(TARGET)

ifeq($(ENV),RAM)
$(TARGET):$(BUILD)
	$(OBJCOPY) -O binary $^ $@	#BUILD生成TARGET的二进制文件(^表示源文件build, @表示目标文件TARGET)
else
$(TARGET):$(BUILD)
	$(OBJCOPY) -O binary $^ $@.TMP	#如果从SD卡启动 先产生.TMP文件
	$(SDTOOLS) $@.TMP $@
endif


$(BUILD):$(COBJS)
	 $(LD) $(LDFLAGS) -o $@ $^	

%.o:%.c 	#任意的.c生成任意的.o
	$(CC) $(CFLAGS) -c -o $@ $^

%.o:%.S 	#汇编文件
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(TARGET) $(BUILD) *.o *.TMP


