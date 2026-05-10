#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__



#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "cc.h"

/* GPIO 中断触发类型枚举 */
typedef enum _gpio_interrupt_mode
{
    kGPIO_NoIntmode = 0,              /* 没有中断 */
    kGPIO_IntLowLevel = 1,             /* 低电平触发 */
    kGPIO_IntHighLevel = 2,            /* 高电平触发 */
    kGPIO_IntRisingEdge = 3,           /* 上升沿触发 */
    kGPIO_IntFallEdge = 5,             /* 下降沿触发 */

}gpio_interrupt_mode_t;

/* 枚举gpio输入输出 */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,
    kGPIO_DigitalOutput = 1U,
}gpio_pin_direction_t;

typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction;
    uint8_t outputlogic;
    gpio_interrupt_mode_t interruptMode;
}gpio_pin_config_t;

void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);
int gpio_pinread(GPIO_Type *base, int pin);

void gpio_enable_interrupt(GPIO_Type *base, int pin);
void gpio_disable_interrupt(GPIO_Type *base, int pin);
int gpio_get_interrupt_flag(GPIO_Type *base, int pin);
void gpio_clear_interrupt_flag(GPIO_Type *base, int pin);
void gpio_interrupt_init(GPIO_Type *base, int pin, gpio_interrupt_mode_t mode);



#endif