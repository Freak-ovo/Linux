#ifndef __BSP_LED__H
#define __BSP_LED__H



#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

#define LED0 0

void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);
void LED_Switch(int led, int status);

#endif