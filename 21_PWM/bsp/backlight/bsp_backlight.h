#ifndef __BACKLIGHT_H__
#define __BACKLIGHT_H__

#include "imx6ull.h"

/* 背光信息结构体 */
typedef struct BackLight_Dev
{
    uint16_t DutyCycle;

} BackLight_Dev_t;



extern BackLight_Dev_t BackLight_Dev;
void BackLight_Init();


#endif /* __BACKLIGHT_H__ */