#include "bsp_led.h"
#include "cc.h"

/* 初始化LED */
void LED_Init(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);  /* 复用GPIO1_IO03 */
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);  /* 设置GPIO1_IO03的电器属性 */
    
    /* GPIO初始化 */
    GPIO1->GDIR = 0x8;                    /* 设置为输出 */
    //GPIO1->DR = 0x0;                      /* 打开LED灯 */
    GPIO1->DR |= (1 << 3);                      /* 默认关闭LED灯 */
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

/* LED灯控制函数 */
void LED_Switch(int led, int status)
{
    switch (led)
    {
        case LED0:
            if (status == ON)
            {
                GPIO1->DR &= ~(1 << 3);       /* bit3清零 */
            }
            else if (status == OFF)
            {
                GPIO1->DR |= (1 << 3);        /* bit3置1 */
            }
    }
}