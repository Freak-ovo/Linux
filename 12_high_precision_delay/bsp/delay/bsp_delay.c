#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_led.h"


/* 延时初始化函数 */
void Delay_Init()
{
    GPT1->CR = 0;   /* 清零 */

    GPT1->CR = (1 << 15);    /* 软件复位 */

    while ((GPT1->CR >> 15) & 0x01);    /* 等待复位完成 */
    /* 
     * 时钟选择为ipg_clk, 预分频器为2, 工作模式为重复计数
     * 默认从0开始计数，计数器每次递增1，当计数器的值等于比较寄存器的值时，产生一个中断，并且计数器重置为0继续计数
     */
    GPT1->CR = (1 << 1) | (1 << 6) | (1 << 5) | (0x01 << 3);   /* 时钟选择为ipg_clk, 预分频器为2, 工作模式为重复计数 */

    /* 分频设置PR寄存器 */
    GPT1->PR = 0;
    GPT1->PR = 65;  /* 66分频 66Mhz/66=1Mhz */

    /* 
     * 1M的频率，计一个数就是1us，0xFFFFFFFF = 4294967295us = 4294s = 71.5分钟
     * 
     */
    /* 配置输出比较通道1 */
    GPT1->OCR[0] = 0xFFFFFFFF; 

#if 0
    /* 打开GPT1 输出比较通道1中断 */
    GPT1->IR = (1 << 0);

    /* 打开GPT1 中断使能GIC */
    GIC_EnableIRQ(GPT1_IRQn);

    /* 注册中断号 */
    sys_irq_handler_t GPT1_IRQn_Handler;
    GPT1_IRQn_Handler.handler = GPT1_IRQHandler;
    GPT1_IRQn_Handler.param = NULL;
    system_register_irq_handler(GPT1_IRQn, GPT1_IRQn_Handler.handler, GPT1_IRQn_Handler.param);

    /* 编写中断处理函数 */

#endif
    GPT1->CR |= (1 << 0);   /* 启动GPT1 */



}
/* GPT1 中断处理函数 */
#if 0
void GPT1_IRQHandler(unsigned int giccIar, void *param)
{
    static unsigned char state = 0;   /* 状态变量，初始值为0 */
    if (GPT1->SR & (1 << 0))   /* 判断是否是输出比较通道1中断 */
    {
        state = !state;   /* 切换状态 */
        LED_Switch(LED0, state);   /* 根据状态控制LED灯的开关 */
    }
    GPT1->SR = (1 << 0);   /* 清除中断标志位 */
} 
#endif

void delay_us(unsigned int us)
{
    unsigned long oldcnt, newcnt;
    unsigned long tcntvalue = 0;

    oldcnt = GPT1->CNT;
    while (1)
    {
        newcnt = GPT1->CNT;
        if (newcnt != oldcnt)
        {
            if (newcnt > oldcnt)
            {
                tcntvalue += newcnt - oldcnt;

            }
            else 
            {
                tcntvalue += 0xFFFFFFFF - oldcnt + newcnt;
            }
            oldcnt = newcnt;
            if (tcntvalue >= us)
            {
                break;
            }
        }
    }
}

/* 延时ms函数 */
void delay_ms(unsigned int ms)
{
    //delay_us(ms * 1000);
    for (int i = 0; i < ms; i++)
    {
        delay_us(1000);
    }
}


/* 短延迟函数 */
void Delay_Short(volatile unsigned int n)
{
    while(n--)
    {

    }
}
/* 延时，再一次循环中大概1ms 
 * 在主频396MHz 下 
 * n: 延时的ms数
 */
void Delay(volatile unsigned int n)
{
    while(n--)
    {
        Delay_Short(0x7ff);
    }
}
