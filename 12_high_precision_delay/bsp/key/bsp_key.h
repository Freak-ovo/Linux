#ifndef __BSP_KEY_H
#define __BSP_KEY_H


#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "bsp_delay.h"



/* 按键值 */
enum Key_Value
{
    KEY_NONE = 0,
    KEY0_VALUE = 1,
};

/* 函数声明 */
void Key_Init(void);
int Key_Read(void);
int Key_GetValue(void);


#endif

