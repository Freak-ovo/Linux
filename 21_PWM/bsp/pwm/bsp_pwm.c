#include "bsp_pwm.h"
#include "bsp_int.h"

/* 初始化 */
void PWM_Init()
{
    PWM1->PWMCR = 0;    /* 清零 */
    PWM1->PWMCR |= (1 << 16);  /* 设置时钟源为ipg_clk = 66MHz */
    PWM1->PWMCR |= (65 << 4);  /* 66MHz/66 =1MHz */
    PWM1->PWMCR |= (1 << 26);  /* FIFO */
    

    PWM1_SetPeriod(1000);/* 设置PWM周期为1000个时钟周期，即1ms */

    /* 设置默认占空比为50% */
    PWM1_SetDutyCycle(50);/* 设置占空比为50个时钟周期，即占空比为50% */


    /* 使能FIFO空中断 */
    PWM1->PWMIR |= (1 << 0);        /* FIFO空中断使能 */

    
    sys_irq_handler_t   pwm1_handler;
    pwm1_handler.handler = pwm1_irq_handler_t;
    pwm1_handler.param = NULL;

    /* 使能中断 */
    GIC_EnableIRQ(PWM1_IRQn);
    /* 注册中断服务函数 */
    system_register_irq_handler(PWM1_IRQn, pwm1_handler.handler, pwm1_handler.param);

    PWM1->PWMSR = 0xFFFF;    /* 清零 */

    /* 打开PWM */
    PWM1->PWMCR |= (1 << 0);  /* 打开PWM */
}
void pwm1_irq_handler_t(unsigned int giccIar, void *param)
{
    if (PWM1->PWMSR & (1 << 3))    /* FIFO空中断标志位 */
    {
        PWM1_SetDutyCycle(50);
        PWM1->PWMSR |= (1 << 3); /* 清除FIFO空中断标志位 */
    }


}

/* 设置PR寄存器 */
void PWM1_SetPeriod(uint16_t PR)
{
    unsigned int regvluae = 0;
    if (PR < 2)
    {
        regvluae = 2;
    }
    else
    {
        regvluae = PR - 2;
    }
    PWM1->PWMPR = (regvluae & 0xFFFF);
    
}

/* 设置占空比 duty 0~200 */
void PWM1_SetDutyCycle(uint16_t duty)
{
    unsigned short period;
    unsigned short sample;
    period = PWM1->PWMPR + 2;
    sample = (unsigned short)(duty * period / 100.0f);
    PWM1->PWMCR = (sample & 0xFFFF);
}
