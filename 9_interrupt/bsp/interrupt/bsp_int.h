#ifndef __INT_H__
#define __INT_H__

#include "imx6ul.h"

typedef void (*system_int_handler_t)(unsigned int giccIar, void *param);

/* 中断处理函数结构体 */
typedef struct _sys_irq_handler
{
    system_int_handler_t handler;   /* 中断处理函数指针 */
    void *param;                     /* 中断处理函数参数 */
} sys_irq_handler_t;


#define NUMBER_OF_IRQ_HANDLERS 160
/* 定义中断处理函数 */
void INT_Init();

void system_register_irq_handler(IRQn_Type irq_num, system_int_handler_t handler, void *param);
void system_irqhandler(unsigned int giccIar);
void default_handler_t(unsigned int giccIar, void *param);

/* 初始化中断控制器 */
void INT_Init();



#endif /* __INT_H__ */
