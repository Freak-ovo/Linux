#ifndef __PWM_H__
#define __PWM_H__



#include "imx6ull.h"



void PWM_Init();
void PWM1_SetPeriod(uint16_t PR);
void PWM1_SetDutyCycle(uint16_t duty);
void pwm1_irq_handler_t(unsigned int giccIar, void *param);



#endif /* __PWM_H__ */