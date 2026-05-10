#include "bsp_exit.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_delay.h"
#include "bsp_beep.h"


/* 初始化 外部中断 GPIO1~18 */
void Exit_Init(void)
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
    gpio1_io18_handler.handler = gpio_io18_irq_handler_t;
    gpio1_io18_handler.param = NULL;

    /* 使能中断 */
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    /* 注册中断服务函数 */
    system_register_irq_handler(GPIO1_Combined_16_31_IRQn, gpio1_io18_handler.handler, NULL);
    /* 使能GPIO18中断 */
    gpio_enable_interrupt(GPIO1, 18);



}
/* 外部中断服务函数 */
void gpio_io18_irq_handler_t(unsigned int giccIar, void *param)
{
    static unsigned char state = 0;
    Delay(10);/* 实际开发中禁止在中断服务函数中使用延时函数，否则会导致中断响应延迟 */
    if (gpio_get_interrupt_flag(GPIO1, 18))
    {
        state = !state;
        Beep_Switch(state); /* 切换蜂鸣器状态 */
        gpio_clear_interrupt_flag(GPIO1, 18); /* 清除中断标志位 */
    }

}