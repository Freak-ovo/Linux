#include "main.h"


/* 使能外设时钟 */

void CLK_Enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;

}
/* 初始化LED */
void LED_Init(void)
{
    IOMUX_SW_MUX->GPIO1_IO03 = 0x5;     /* 复用GPIO1_IO03 */
    IOMUX_SW_PAD->GPIO1_IO03 = 0x10b0;  /* 设置GPIO1_IO03的电器属性 */

    /* GPIO初始化 */
    GPIO1->GDIR = 0x8;                    /* 设置为输出 */
    GPIO1->DR = 0x0;                      /* 打开LED灯 */
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

/* 打开LED */
void LED_ON(void)
{
    GPIO1->DR &= ~(1 << 3);       /* bit3清零 */
}

/* 关闭LED */
void LED_OFF(void)
{
    GPIO1->DR |= (1 << 3);        /* bit3置1 */
}

int main(void)
{
    /* 初始化时钟使能 */
    CLK_Enable();

    /* 初始化LED */
    LED_Init();

    /* 设置LED闪烁 */
    while(1)
    {
        LED_ON();
        Delay(500);

        LED_OFF();
        Delay(500);

    }


}

