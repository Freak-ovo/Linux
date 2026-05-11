# 指定交叉编译器的前缀，使用 ?= 允许外部传入覆盖
CROSS_COMPILE	?= arm-linux-gnueabihf-
# 指定最终生成的目标文件名，默认为 print
TARGET			?= RTC

CC				:= $(CROSS_COMPILE)gcc  # 定义 C 编译器完整命令
LD				:= $(CROSS_COMPILE)ld   # 定义链接器完整命令
OBJCOPY			:= $(CROSS_COMPILE)objcopy # 定义格式转换工具，用于将 ELF 转为 BIN
OBJDUMP			:= $(CROSS_COMPILE)objdump # 定义反汇编工具


LIBPATH := -lgcc -L /usr/lib/gcc-cross/arm-linux-gnueabihf/15

# 头文件所在的目录集合，使用 \ 进行换行连接
INCLUDIRS		:= imx6ull \
					stdio/include\
					bsp/clk \
					bsp/delay \
					bsp/led	\
					bsp/beep \
					bsp/key	\
					bsp/gpio \
					bsp/exit \
					bsp/epit \
					bsp/keyfilter \
					bsp/int	\
					bsp/lcd \
					bsp/uart \
					bsp/rtc
					

# 源文件（.c 和 .S）所在的目录集合
SRCDIRS			:= project \
					stdio/lib \
					bsp/clk \
					bsp/delay \
					bsp/led	\
					bsp/beep \
					bsp/key	\
					bsp/gpio \
					bsp/exit \
					bsp/epit \
					bsp/keyfilter \
					bsp/int	\
					bsp/lcd \
					bsp/uart \
					bsp/rtc
					

# 将头文件目录加上 -I 前缀，编译时告诉编译器去哪里找头文件
INCLUDES		:= $(patsubst %, -I %, $(INCLUDIRS)) 

# （注：这一行看起来是之前的手误拼写，实际在下方没有被使用）
SFILUDE			:= $(patsubst %, %.S, $(INCLUDIRS))

# 使用 foreach 和 wildcard 函数，自动遍历并找出所有目录下的 .S 和 .c 文件
SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

# 使用 notdir 去掉文件的路径，只保留纯文件名
SFILENDIR		:= $(notdir $(SFILES))
CFILENDIR		:= $(notdir $(CFILES))

# 将纯文件名转换为 obj/xxx.o 的形式，指定编译后的目标文件存放位置
SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))

# 汇总所有的 .o 目标文件
OBJS 			:= $(SOBJS)$(COBJS)

# 指定搜索路径，make 会自动去这些目录下寻找依赖的源文件
VPATH			:= $(SRCDIRS)

# 声明伪目标，防止与当前目录下的同名文件产生冲突
.PHONY : clean

# 最终目标规则：链接所有 .o 文件生成 .elf，再转换为 .bin 和 .dis 反汇编文件
$(TARGET).bin : $(OBJS)
	$(LD) -Timx6ull.lds -o $(TARGET).elf $^ $(LIBPATH)
	$(OBJCOPY) -O binary -S $(TARGET).elf $@ 
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

# 定义 CPU 宏，用于在代码中进行条件编译
CPU_DEF := -DCPU_MCIMX6Y2CVM08

# 汇编文件（.S）的编译规则
$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -O2 $(CPU_DEF) $(INCLUDES) -o $@ $<

# C 文件（.c）的编译规则
$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -Wa,-mimplicit-it=thumb -nostdlib -fno-builtin -c -O2 $(INCLUDES) -o $@ $<

# 清理规则：删除编译过程中生成的所有中间文件和最终目标文件
clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(OBJS)

# 打印调试信息，执行 make print 可以查看各个变量的实际解析结果
print :
	@echo INCLUDES = $(INCLUDES)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILENDIR = $(SFILENDIR)
	@echo CFILENDIR = $(CFILENDIR)
	@echo SOBJS = $(SOBJS)
	@echo COBJS = $(COBJS)
	@echo OBJS = $(OBJS)