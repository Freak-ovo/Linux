#include "bsp_keyfilter.h"
#include "bsp_int.h"
#include "bsp_gpio.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_epit.h"
/* 初始化 */
void KeyFilter_Init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);  /* 复用IO */

    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0X080);  /* 设置IO电器属性 */

    /* GPIO初始化 */
    //GPIO1->GDIR  &= ~(1 << 18);    /* 将bit18设置为0，设置为输入 */
    gpio_pin_config_t gpio_pin_config;
    gpio_pin_config.direction = kGPIO_DigitalInput;
    gpio_pin_config.outputlogic = 0;
    gpio_pin_config.interruptMode = kGPIO_IntFallEdge;
    gpio_init(GPIO1, 18, &gpio_pin_config);


    sys_irq_handler_t   gpio1_io18_handler;
    gpio1_io18_handler.handler = keyfilter_irq_handler_t;
    gpio1_io18_handler.param = NULL;

    /* 使能中断 */
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    /* 注册中断服务函数 */
    system_register_irq_handler(GPIO1_Combined_16_31_IRQn, gpio1_io18_handler.handler, gpio1_io18_handler.param);
    /* 使能GPIO18中断 */
    gpio_enable_interrupt(GPIO1, 18);

    /* 初始化定时器 */
    keyfilter_timer_init(66000000 / 1000 * 10); /* 10ms的定时器加载值 */

}
/* 初始化定时器 */
void keyfilter_timer_init(unsigned int value)
{

    /* 配置EPIT1_CR寄存器 */
    EPIT1->CR = 0;

    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 24);

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
/* 关闭定时器 */
void keyfilter_timer_stop(void)
{
    /* 关闭定时器 */
    EPIT1->CR &= ~(1 << 0); /* 关闭定时器 */
}

/* 重启定时器 */
void keyfilter_timer_restart(unsigned int value)
{
    EPIT1->CR &= ~(1 << 0); /* 关闭定时器 */
    /* 重新加载定时器 */
    EPIT1->LR = value;  /* 加载值，计数器计数到 0 时，重新从此值开始计数 */
    /* 启动定时器 */
    EPIT1->CR |= (1 << 0); /* 启动定时器 */
}
/* EPIT1 中断处理函数 */
void keyfilter_EPIT1_IRQHandler(unsigned int giccIar, void *param)
{
    static unsigned char state = OFF;
    if (EPIT1->SR & (1 << 0)) /* 判断是否是EPIT1中断 */
    {
        keyfilter_timer_stop(); /* 关闭定时器 */

        if (gpio_pinread(GPIO1, 18) == 0) /* 判断按键是否按下 */
        {
            state = !state;
            Beep_Switch(state);    /* 切换蜂鸣器状态 */
        }
    }
    /* 清除中断标志位 */
    EPIT1->SR |= (1 << 0); 
}
/* 按键消抖中断服务函数 */
void keyfilter_irq_handler_t(unsigned int giccIar, void *param)
{
    unsigned int value = 66000000 / 1000 * 10; /* 10ms的定时器加载值 */
    /* 开启定时器 */
    keyfilter_timer_restart(value); /* 重启定时器，设置加载值 */
    /* 清除中断标志位 */
    gpio_clear_interrupt_flag(GPIO1, 18);

    /* 处理按键事件 */

}
