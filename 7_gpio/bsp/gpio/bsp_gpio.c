#include "bsp_gpio.h"


/* 初始化GPIO */
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
    if (config->direction == kGPIO_DigitalInput)    /* 输入模式 */
    {
        base->GDIR &= ~(1U << pin);
    }
    else
    {
        base->GDIR |= (1U << pin);
        /* 设置输出电平为默认值 */
        gpio_pinwrite(base, pin, config->outputlogic);
    }
}

/* 设置GPIO输出电平 */
void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
    if (value == 0)    /* 低电平 */
    {
        base->DR &= ~(1U << pin);
    }
    else    /* 高电平 */
    {
        base->DR |= (1U << pin);
    }
}


/* 获取GPIO输入电平 */
int gpio_pinread(GPIO_Type *base, int pin)
{
    return (((base->DR) >> pin) & 0x01);
}
