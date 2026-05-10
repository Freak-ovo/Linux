#include "bsp_uart.h"
#include "bsp_gpio.h"

/*
 * @description : 初始化串口1,波特率为115200
 * @param       : 无
 * @return      : 无
 */
void UART_Init()
{
    /* 1、初始化串口IO */
    UART_IO_Init();

    /* 2、初始化UART1 */
    UART_Close(UART1);    /* 先关闭UART1 */
    UART_SoftReset(UART1);  /* 软件复位UART1 */

    UART1->UCR1 = 0;        /* 先清除UCR1寄存器 */
    UART1->UCR1 &= ~(1<<14); /* 关闭自动波特率检测 */
#if 0
    /*
     * 设置UART的UCR2寄存器，设置字长，停止位，校验模式，关闭硬件流控
     * bit14: 1 忽略RTS引脚
     * bit8:  0 关闭奇偶校验
     * bit6:  0 1位停止位
     * bit5:  1 8位数据位
     * bit2:  1 打开发送
     * bit1:  1 打开接收
     */
    UART1->UCR2 |= (1<<14) | (1<<5) | (1<<2) | (1<<1);
    UART1->UCR3 |= 1<<2;    /* UCR3的bit2必须为1 */

    /*
     * 设置波特率
     * 波特率计算公式: Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1))
     * 如果要设置波特率为115200，那么可以使用如下参数:
     * Ref Freq = 80M 也就是寄存器UFCR的bit9:7=101, 表示1分频
     * UBMR = 3124
     * UBIR = 71
     * 因此波特率= 80000000/(16 * (3124+1)/(71+1))
     *           = 80000000/(16 * 3125/72)
     *           = (80000000*72) / (16*3125)
     *           = 115200
     */
    UART1->UFCR = 5 << 7;  /* ref freq等于ipg_clk/1=80Mhz */
    UART1->UBIR = 71;
    UART1->UBMR = 3124;
#endif

    UART_SetBaudrate(UART1, 115200, 80000000); /* 设置波特率 */


    UART_Open(UART1);     /* 使能串口 */
}
/* 
 * @description : 初始化串口1所使用的IO引脚
 * @param       : 无
 * @return      : 无
 */
void UART_IO_Init()
{
    /* 1、初始化串口IO
     * UART1_RXD -> UART1_TX_DATA 
     * UART1_TXD -> UART1_RX_DATA 
     */
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);
}

/* 
 * @description : 波特率计算公式，
 *                可以用此函数计算出指定串口对应的UFCR，
 *                UBIR和UBMR这三个寄存器的值
 * @param - base : 要计算的串口。
 * @param - baudrate : 要使用的波特率。
 * @param - srcclock_hz : 串口时钟源频率，单位Hz
 * @return : 无
 */
void UART_SetBaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u;
    uint32_t denominator = 0U;
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0)
    {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK))
    {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator)
        {
            numerator = 1;
        }
        if (0 == denominator)
        {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider)
    {
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated 
     * baud rate. Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator). 
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0)
    {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3)
    {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1);
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }
}






/* 关闭串口 */
void UART_Close(UART_Type *base)
{
    base->UCR1 &= ~(1 << 0);  /* 关闭串口 */
}

/* 打开串口 */
void UART_Open(UART_Type *base)
{
    base->UCR1 |= (1 << 0);  /* 打开串口 */
}

/* 复位UART */
void UART_SoftReset(UART_Type *base)
{
    base->UCR2 |= (1 << 0);  /* 复位UART */
    while ((base->UCR2 & 0x01) == 0);  /* 等待复位完成 */
}

/* 串口读数据 */
uint8_t UART_Read(UART_Type *base)
{
    while ((base->USR2 & (1 << 2)) == 0);  /* 等待接收数据 */

    return base->URXD;  /* 返回接收数据 */


}


/* 串口写一个数据 */
void UART_Write(UART_Type *base, uint8_t data)
{
    while ((base->USR2 & (1 << 3)) == 0);  /* 等待发送缓冲区空 */

    base->UTXD = data;  /* 写入数据 */

}

/* 串口写一串字符串 */
void UART_WriteString(UART_Type *base, const unsigned char *str)
{
    while (*str)
    {
        UART_Write(base, (uint8_t)(*str));  /* 逐个发送字符串中的字符 */
        str++;
    }
}



/* 定义raise函数，防止编译报错 */
void raise(int sig)
{

}
