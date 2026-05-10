#ifndef __BSP_DELAY__H
#define __BSP_DELAY__H



#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

void Delay_Short(volatile unsigned int n);
void Delay(volatile unsigned int n);

#endif