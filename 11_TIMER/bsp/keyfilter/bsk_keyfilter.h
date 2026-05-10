#ifndef __BSK_KEYFILTER_H__
#define __BSK_KEYFILTER_H__


#include "imx6ul.h"


void KeyFilter_Init(void);
void keyfilter_timer_init(unsigned int value);
void keyfilter_timer_stop(void);
void keyfilter_timer_restart(unsigned int value);
void keyfilter_EPIT1_IRQHandler(unsigned int giccIar, void *param);
void keyfilter_irq_handler_t(unsigned int giccIar, void *param);



#endif /* __BSK_KEYFILTER_H__ */


