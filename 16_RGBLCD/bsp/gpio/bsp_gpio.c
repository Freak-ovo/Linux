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
    gpio_interrupt_init(base, pin, config->interruptMode); /* 配置GPIO中断 */
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


/* 使能指定IO中断*/
void gpio_enable_interrupt(GPIO_Type *base, int pin)
{
    /* 使能中断 */
    base->IMR |= (1 << pin);
}

/* 禁用指定IO中断*/
void gpio_disable_interrupt(GPIO_Type *base, int pin)
{
    /* 禁用中断 */
    base->IMR &= ~(1 << pin);
}

/* 清除中断标志位 */
void gpio_clear_interrupt_flag(GPIO_Type *base, int pin)
{
    /* 清除中断标志位 */
    base->ISR &= ~(1 << pin);
}

/* 获取中断标志位 */
int gpio_get_interrupt_flag(GPIO_Type *base, int pin)
{
    return (((base->ISR) >> pin) & 0x01);
}

/* GPIO 中断初始化函数 */
void gpio_interrupt_init(GPIO_Type *base, int pin, gpio_interrupt_mode_t mode)
{
    volatile uint32_t *icr;
    uint32_t icrShift;
    icrShift = pin;
     /* 配置GPIO为输入模式 */
    /* 配置中断触发方式 */
    base->EDGE_SEL &= ~(1U << pin); /* 默认使用边沿触发 */

    /* 配置中断触发方式 */
    if (pin < 16)
    {
        icr = &(base->ICR1);    /* 0-15 */
    }
    else
    {
        icr = &(base->ICR2);    /* 16-31 */
        icrShift -= 16;
    }
    switch (mode)
    {
        case kGPIO_NoIntmode:
            break;
        case kGPIO_IntLowLevel:
            *icr &= ~(3 << (2 * icrShift)); /* 低电平触发 */
            break;
        case kGPIO_IntHighLevel:
            *icr &= ~(3 << (2 * icrShift)); /* 清零 */
            *icr |= (1 << (2 * icrShift)); /* 高电平触发 */
            break;
        case kGPIO_IntRisingEdge:
            *icr &= ~(3 << (2 * icrShift)); /* 清零 */
            *icr |= (2 << (2 * icrShift)); /* 边沿触发 */
            break;
        case kGPIO_IntFallEdge:
            *icr &= ~(3 << (2 * icrShift)); /* 清零 */
            *icr |= (3 << (2 * icrShift)); /* 下降沿触发 */
            break;
        default:
            break;
    }
}