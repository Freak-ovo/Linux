#include "bsp_beep.h"
#include "cc.h"
/*
 * BEEP控制IO为SNVS_TAMPER1 当输出高电平时蜂鸣器关闭 输出低电平时蜂鸣器打开
 * 1. 初始化SNVS_TAMPER1这个IO 复用为GPIO_IO01
 * 2. 设置SNVS_TAMPER1这个IO的电器属性
 * 3. 初始化GPIO
 * 4. 控制GPIO 输出高低电平
 *
*/
/* BEEP初始化 */
void Beep_Init(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO05_GPIO1_IO05, 0);  /* 复用IO */

    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO05_GPIO1_IO05, 0x10B0);  /* 设置IO电器属性 */

    /* GPIO初始化 */
    GPIO1->GDIR  |= (1 << 1);    /* 将bit1设置为1设置为输出 */
    GPIO1->DR    |= (1 << 1);   /* 默认关闭 */

}

/* 蜂鸣器控制函数 */
void Beep_Switch(int status)
{
    if (status == OFF)
    {
        GPIO1->DR |= (1 << 1);
    }
    else if (status == ON)
    {
        GPIO1->DR &= ~(1 << 1);   
    }
}