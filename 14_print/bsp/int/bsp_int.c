#include "bsp_int.h"

/* 中断嵌套计数器 */
static unsigned int ieqNesting;



/* 中断处理函数数组 */
static sys_irq_handler_t sys_irq_vector_table[NUMBER_OF_INT_VECTORS];

/* 初始化中断向量表 */
static void init_irq_vector_table(void)
{
    unsigned int i;
    ieqNesting = 0;/* 清零中断嵌套计数器 */
    for (i = 0; i < NUMBER_OF_INT_VECTORS; i++)
    {
        sys_irq_vector_table[i].handler = default_handler_t;
        sys_irq_vector_table[i].param = NULL;


    }

}
/* 注册中断处理函数 */
void system_register_irq_handler(IRQn_Type irq_num, system_int_handler_t handler, void *param)
{
    if (irq_num < NUMBER_OF_INT_VECTORS)
    {
        sys_irq_vector_table[irq_num].handler = handler;
        sys_irq_vector_table[irq_num].param = param;
    }
}



/* 中断处理函数 */
void INT_Init()
{
    GIC_Init();
    init_irq_vector_table();
    /* 中断向量偏移设置 */
    __set_VBAR(0x87800000);

    

}



/* 具体中断处理函数 */
void system_irqhandler(unsigned int giccIar)
{
    uint32_t intNum = giccIar &= 0x3ff; /* 获取中断ID号 */  
    /* 检查中断ID是否有效 */
    if (intNum < NUMBER_OF_INT_VECTORS)
    {
        sys_irq_vector_table[intNum].handler(giccIar, sys_irq_vector_table[intNum].param);
    }
    ieqNesting++;/* 中断嵌套计数器增加 */

    /* 根据中断ID号，读取中断处理函数，然后执行该函数 */
    sys_irq_vector_table[intNum].handler(giccIar, sys_irq_vector_table[intNum].param);
    ieqNesting--;/* 中断嵌套计数器减少 */
}




/* 默认中断处理函数 */
void default_handler_t(unsigned int giccIar, void *param)
{
    while (1)
    {
        /* 无限循环，等待调试 */
    }
}
void default_handler(unsigned int giccIar, void *param)
{

}