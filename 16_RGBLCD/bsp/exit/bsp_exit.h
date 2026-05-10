#ifndef __BSP_EXIT_H__
#define __BSP_EXIT_H__

#include "imx6ull.h"

void Exit_Init(void);
void gpio_io18_irq_handler_t(unsigned int giccIar, void *param);

#endif /* __BSP_EXIT_H__ */