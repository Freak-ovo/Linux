#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__



#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "cc.h"



/* 枚举gpio输入输出 */
typedef enum _gpio_pin_direction
{
    INPUT = 0U,
    OUTPUT = 1U,
}gpio_pin_direction_t;

typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction;
    uint8_t outputlogic;
}gpio_pin_config_t;

void GPIO_Init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);
int gpio_pinread(GPIO_Type *base, int pin);




#endif