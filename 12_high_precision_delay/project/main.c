#include "fsl_common.h"    /* 引入NXP官方SDK的通用宏定义和基础类型 */
#include "fsl_iomuxc.h"    /* 引入IOMUXC（引脚复用）相关的寄存器配置函数 */
#include "MCIMX6Y2.h"      /* 引入I.MX6ULL芯片的核心寄存器映射头文件 */
#include "core_ca7.h"

#include "bsp_clk.h"       /* 引入自定义的板级时钟驱动头文件 */
#include "bsp_delay.h"     /* 引入自定义的板级延时驱动头文件 */
#include "bsp_led.h"       /* 引入自定义的板级LED驱动头文件 */
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"       /* 引入自定义的板级中断驱动头文件 */
#include "bsp_exit.h"
#include "bsp_epit.h"
#include "bsk_keyfilter.h"


/* 程序主入口，裸机程序的起点 */
int main(void)
{
    INT_Init();        /* 初始化中断控制器，设置中断向量表地址 */
    CLK_Enable(); 
    Delay_Init();
    LED_Init();
    Beep_Init();
    Key_Init();
    GIC_Init();
    Exit_Init();
    EPIT1_Init(0, 33000000);/* 500ms 周期的定时器 */
    KeyFilter_Init();

    unsigned char led_state = OFF;


    while (1)
    {
        
        led_state = !led_state;
        LED_Switch(LED0, led_state);
        delay_ms(500);

    }
}