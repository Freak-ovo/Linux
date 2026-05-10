#include "bsp_delay.h"


/* 短延迟函数 */
void Delay_Short(volatile unsigned int n)
{
    while(n--)
    {

    }
}
/* 延时，再一次循环中大概1ms 
 * 在主频396MHz 下 
 * n: 延时的ms数
 */
void Delay(volatile unsigned int n)
{
    while(n--)
    {
        Delay_Short(0x7ff);
    }
}
