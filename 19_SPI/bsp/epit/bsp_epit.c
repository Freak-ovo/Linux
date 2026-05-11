#include "bsp_epit.h"
#include "bsp_int.h"
#include "bsp_gpio.h"
#include "bsp_led.h"


/* EPIT 初始化 参数1： 分频系数 参数2： 加载值 */
void EPIT1_Init(unsigned int frac, unsigned int value)
{
    /*  */
    if (frac > 4095)
    {
        frac = 4095;
    }

    /* 配置EPIT1_CR寄存器 */
    EPIT1->CR = 0;

    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (frac << 4) | (1 << 24);

    /* 配置EPIT1_LR寄存器 */
    EPIT1->LR = value;  /* 加载值，计数器计数到 0 时，重新从此值开始计数 */

    /* 比较寄存器 */
    EPIT1->CMPR = 0; /* 比较寄存器，计数器计数到此值时，产生中断 */

    /* 初始化中断 */
    GIC_EnableIRQ(EPIT1_IRQn); /* 使能EPIT1中断 */
    /* 注册中断处理函数 */
    sys_irq_handler_t EPIT1_irq_handler;
    EPIT1_irq_handler.handler = EPIT1_IRQHandler;
    EPIT1_irq_handler.param = NULL;
    system_register_irq_handler(EPIT1_IRQn, EPIT1_irq_handler.handler,EPIT1_irq_handler.param);

    /* 启动定时器 */
    EPIT1->CR |= (1 << 0); /* 启动定时器 */

}


/* EPIT 中断处理函数 */
void EPIT1_IRQHandler(unsigned int giccIar, void *param)
{
    static unsigned char state = 0;
    state = !state;
    if (EPIT1->SR & (1 << 0)) /* 判断是否是EPIT1中断 */
    {
        LED_Switch(LED0, state);    /* 切换LED0状态 */
    }
    /* 清除中断标志位 */
    EPIT1->SR |= (1 << 0); 


}

