# UART
## 一、串口协议简介
>串口全称叫做串行接口，通常也叫做COM 接口，串行接口指的是数据一个一个的顺序传输，通信线路简单。使用两条线即可实现双向通信，一条用于发送，一条用于接收。串口通信距离远，但是速度相对会低，串口是一种很常用的工业接口。I.MX6U 自带的UART 外设就是串口的一种，UART 全称是Universal Asynchronous Receiver/Trasmitter，也就是异步串行收发器。既然有异步串行收发器，那肯定也有同步串行收发器，学过STM32 的同学应该知道，STM32除了有UART 外， 还有另外一个叫做USART 的东西。USART 的全称是UniversalSynchronous/Asynchronous Receiver/Transmitter，也就是同步/异步串行收发器。相比UART 多了一个同步的功能，在硬件上体现出来的就是多了一条时钟线。一般USART 是可以作为UART使用的，也就是不使用其同步的功能。


## 二、IMX6Ull 串口 UART 原理
> IMX6Ull 的 UART_URXD 寄存器保存串口接收到的数据。
> UART_UTXD 寄存器为发送数据寄存器，如果需要通过串口发送数据，只需要将数据写进 UART_UTXD 寄存器中。
> UART_UCR1~UCR4 都是串口控制寄存器，bit[0] 是 UART 的使能位，为 1 的时候使能 UART，bit[14] 为自动检测波特率使能位，为 1 的时候是能波特率
> UART_UCR2 bit[0] 是 UART 软件复位，为 0 时复位UART，bit[1] 是接收使能，要配置为1。bit[2] 是发送使能，要配置为1。bit[5] 是数据长度为0时7位，1时8位数据，bit[6] 是停止位设置，0表示 1停止位，1表示 2停止位。bit[7] 是奇偶校验位，为0时偶检验，为1时奇校验。
> UART_UCR3 bit[2] 必须一直置1。
> UART_UFCR bit[9:7] 寄存器，设置分频值，UART的时钟源 = PLL / 6 = 480MHz/6 = 80MHz。UART_CLK_SET 位 设置UART的时钟源，为 0 的时候UART的时钟源为80MHz，为1的时候为24MHz晶振。CSCDR1寄存器的UART_CLK_PODF位控制分频。一般设置为1分频，因此UART_CLK_ROOT = 80MHz。
> UART_UFCR、UART_UBCR 和 UART_UBMR 寄存器决定了串口波特率：
> $BaudRate = \frac{Ref\,Freq}{16 \times \frac{UBMR+1}{UBIR+1}}$
> UART_USR2 的bit[0] 为1时表示有数据可以读取，bit[3] 为1的时表示数据发送完成。
## 三、实验程序编写
> UART1_TXD 使用的 IO 为 UART1_TX_DATA, UART1_RXD 所使用的IO为 UART1_RX_DATA
