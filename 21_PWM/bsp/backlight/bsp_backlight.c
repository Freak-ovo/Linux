#include "bsp_backlight.h"
#include "bsp_pwm.h"

BackLight_Dev_t BackLight_Dev;

/* 初始化 */
void BackLight_Init()
{
    unsigned char i = 0;
    /* 初始化IO */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_PWM1_OUT, 1);  /* 复用PWM1_OUT */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_PWM1_OUT, 0xB090);  /* 设置IO电器属性 */


    /* PWM初始化 */

    PWM_Init();
    /* 设置默认占空比为50% */
    BackLight_Dev.DutyCycle = 50;
    
    for (i = 0; i < 4; i++)
    {
        PWM1_SetDutyCycle(BackLight_Dev.DutyCycle);
    }


}