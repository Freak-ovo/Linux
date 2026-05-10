#ifndef __BSP_EPIT_H__
#define __BSP_EPIT_H__



void EPIT1_Init(unsigned int frac, unsigned int value);
void EPIT1_IRQHandler(unsigned int giccIar, void *param);


#endif /* __BSP_EPIT_H__ */
