#include "fsl_common.h"    /* 引入NXP官方SDK的通用宏定义和基础类型 */
#include "fsl_iomuxc.h"    /* 引入IOMUXC（引脚复用）相关的寄存器配置函数 */
#include "MCIMX6Y2.h"      /* 引入I.MX6ULL芯片的核心寄存器映射头文件 */

#include "bsp_clk.h"       /* 引入自定义的板级时钟驱动头文件 */
#include "bsp_delay.h"     /* 引入自定义的板级延时驱动头文件 */
#include "bsp_led.h"       /* 引入自定义的板级LED驱动头文件 */
#include "bsp_beep.h"
#include "bsp_key.h"

/* 程序主入口，裸机程序的起点 */
int main(void)
{
    
    CLK_Enable(); 
    LED_Init();
    Beep_Init();
    Key_Init();

    int i = 0;

    unsigned char led_state = OFF;
    unsigned char beep_state = OFF;
    unsigned char key_value = KEY_NONE;
    unsigned char key_state = KEY_NONE;

    while (1)
    {


        key_value = Key_GetValue();
        if (key_value)  /* 当key=value=1时 表示按键有效 */
        {
            switch (key_value)
            {
                case KEY0_VALUE:
                    beep_state = !beep_state;
                    Beep_Switch(beep_state);
                    break;
                case KEY_NONE:
                    key_state = KEY_NONE;
                    break;
            }
        }
        ++i;
        if (i % 50 == 0)
        {
            led_state = !led_state;
            LED_Switch(LED0, led_state);
        }
        Delay(10);
    }
}