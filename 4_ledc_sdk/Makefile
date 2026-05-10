CROSS_COMPILE	?= arm-linux-gnueabihf-
NAME			?= ledc

CC				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY			:= $(CROSS_COMPILE)objcopy
OBJDUMP			:= $(CROSS_COMPILE)objdump

# 真实 SDK 路径（只需要 devices 和 CMSIS 路径）
SDK_ROOT		:= /home/leo/mcuxpresso_sdk
INCS			:= -I$(SDK_ROOT)/devices/MCIMX6Y2 \
			   -I$(SDK_ROOT)/CMSIS/Core/Include

# CPU 宏仍然需要，因为 MCIMX6Y2.h 可能依赖它
CPU_DEF			:= -DCPU_MCIMX6Y2CVM08

CFLAGS			:= -Wall -nostdlib -c -O2 $(CPU_DEF) $(INCS)

OBJS			:= start.o main.o

$(NAME).bin: $(OBJS)
	$(LD) -Timx6ull.lds -o $(NAME).elf $^
	$(OBJCOPY) -O binary -S $(NAME).elf $@ 
	$(OBJDUMP) -D -m arm $(NAME).elf > $(NAME).dis

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

%.o : %.S
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(NAME).bin $(NAME).elf $(NAME).dis