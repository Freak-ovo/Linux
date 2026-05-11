# SPI(Serial Peripheral Interface)
## 一、 SPI 协议
> 1. SPI相比 I2C 最大的优势有2点，最高可以达几十MHz，甚至上百MHz， 第二就是全双工。
> 2. SPI 接口和 I2C1 一样，一个SPI接口可以连接多个SPI外设，SPI通过CS引脚/数据线，片选引脚选择和那个SPI外设来通信。SPI通信先将指定的SPI外设对应的CS引脚来选择此设备。
### SPI 总线定义
> 1. CS/SS，Slave Select/Chip Select，这个是片选信号线，用于选择需要进行通信的从设备。
> 2. SCK，Serial Clock，串行时钟，和I2C 的SCL 一样，为SPI 通信提供时钟。
> 3. MOSI/SDO，Master Out Slave In/Serial Data Output，简称主出从入信号线，这根数据线只能用于主机向从机发送数据，也就是主机输出，从机输入。
> 4. MISO/SDI，Master In Slave Out/Serial Data Input，简称主入从出信号线，这根数据线只能用户从机向主机发送数据，也就是主机输入，从机输出。
> ![alt text](images/image.pngimage.png)
### APLHA开发板通过ECSPI3接口极了一个6周传感器：
> ECSPI3_SCK: UART2_RXD
> ECSPI3_MOSI: UART2_CTS
> ECSPI3_SS0: UART2_TXD
> ECSPI3_SDA: UART2_RTS
>IMX6ULL 一个SPI接口有4个硬件片选，分别是SS0~3。
## 二、IMX6ULL SPI 接口配置
> 1. IMX6ULL 的SPI 接口叫做 ECSPI，支持全双工、主从可配置。
> 2. 4个硬件片选信号，可以使用软件片选，这样一个SPI 接口所能连接的外设就无限制了。
> 3. 寄存器配置
> > 1. RXDATA 寄存器 用于接收数据。
> > 2. TXDATA 寄存器 用于发送数据。
> > 3. CONREG 寄存器 用于配置SPI 接口。bit[0] 为使能位，使用时置[1]使能SPI，bit[3] 置[1] 表示当向TXFIFO 写入数据后马上开启SPI 突发访问，也就是发送数据。bit[7:4] 设置SPI通道主从模式。bit[7] 为通道3，bit[4]为通道0。我们使用导SS0 也就是通道0。也就是要设置bit[4]为[1]。bit[19:18] 设置为00，bit[31:30]设置突发访问长度，我们设置为7也就是8bit一个字节。
> > 4. CONFIGREG 寄存器的bit[0] 为PHA，设置为0，表示 串行时钟第一个跳变沿来开始采集数据，设置bit[4]为PO,设置为0，表示SCLK空闲时为低电平。bit[8] 设置为0。bit[15:12]设置片选信号，设置为0。bit[19:16]设置为0，表示空闲的时候数据线为高电平。bit[20]设置为0，表示SCLK空闲时候为低。
> > 5. STATREG 寄存器 用于查询SPI 状态。bit[0]等待TXFIFO 为空。我那在发送数据之前需要等待TXFIFO为空。也就是要等待bit[0]为1，bit[3]表示RXFIFO是否有数据，为1的时候表示RXFIFO至少有一个字的数据，我们在接收数据的时候需要等待bit[3]为1。否则会丢失数据。
> > 6. PREIODREG 寄存器，bit[14:0] 设置wait states 时间，我们设置0x200，bit[15]设置wait states 的时钟源是SPICLK。bit[21:16]表示片选信号的延迟，可设置0~23,这里可以设置为0。
> > 7. SPI时钟源设置：
> SPI时钟源最终来源于 PLL3_SW_CLK = 480MHz / 8 = 60MHz，CSCDR1 的bit[18] 为0，表示使用60MHz 时钟源。CSCDR2bit[24:19]设置为0，表示1分频，因此最终进入到SPI时钟源为60MHz。
> ESPI模块还需要对时钟进行2级分频，由ECSPI_CONREG 寄存器设置。bit[15:12]设置前级分频，可以设置0~0xf，表示1~16分频，设置二级分频由bit[11:8],设置2^n分频，n = 0~15。
## 三、ICM2068简介
> 1. 6轴传感器，3轴陀螺仪，3轴加速度器。
> 2. 陀螺仪分辨率可设置，加速度分辨率可设置。
> 3. 陀螺仪和加速度都是16位的adc。
> 4. 通信接口为I2C 或者 SPI ,I2C速度400KHz，SPI 速度 800MHz。
> 5. ICM20608 的 WHO_AM_I 寄存器地址为0x75。默认值位0xAF，可能是0xAE。
> +-250 =500s 2^16 = 66535 65535/500 = 131.02

## 四、实验程序编写
> 1. 我们在使用浮点计算的时候程序卡死，因为我们没有开始IMX6ULL的浮点运算，我们在编译的时候也没有使用浮点运算。解决此问题需要俩点：
> > 1. 开启IMX6ULL的浮点单元。
> > 2. 编译的时候使用指定硬件浮点运算。