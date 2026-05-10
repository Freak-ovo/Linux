#include "fsl_common.h"    /* 引入NXP官方SDK的通用宏定义和基础类型 */
#include "fsl_iomuxc.h"    /* 引入IOMUXC（引脚复用）相关的寄存器配置函数 */
#include "MCIMX6Y2.h"      /* 引入I.MX6ULL芯片的核心寄存器映射头文件 */
#include "core_ca7.h"

#include "bsp_clk.h"       /* 引入自定义的板级时钟驱动头文件 */
#include "bsp_delay.h"     /* 引入自定义的板级延时驱动头文件 */
#include "bsp_uart.h"      /* 引入自定义的板级串口驱动头文件 */

#include "bsp_led.h"       /* 引入自定义的板级LED驱动头文件 */
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"       /* 引入自定义的板级中断驱动头文件 */
#include "bsp_exit.h"
#include "bsp_epit.h"
#include "bsp_keyfilter.h"
#include "bsp_lcd.h"
#include "stdio.h"







/* 程序主入口，裸机程序的起点 */
int main(void)
{
    INT_Init();        /* 初始化中断控制器，设置中断向量表地址 */
    CLK_Enable(); 
    UART_Init();
    LED_Init();
    Beep_Init();
    Key_Init();
    GIC_Init();
    Exit_Init();
    EPIT1_Init(0, 33000000);/* 500ms 周期的定时器 */
    KeyFilter_Init();
    LCD_Init();

    LCD_INFO_t TFT_LCD_DEV;
    LCD_DrawPoint(0, 0, LCD_BLUE);  /* 绘制一个蓝色的点在(0,0) 左上方 */
    LCD_DrawPoint(0,TFT_LCD_DEV.Width-1, LCD_GREEN);  /* 绘制一个绿色的点在(0,Width-1) 左上方 */
    LCD_DrawPoint(TFT_LCD_DEV.Width-1, TFT_LCD_DEV.Height-1, LCD_YELLOW);  /* 绘制一个黄色的点在(Width-1,Height-1) 右下方 */



    LCD_DrawPoint(100, 100, LCD_RED);
    printf("x = %d, y = %d, color = 0x%08X\n", 100, 100, LCD_ReadPoint(100, 100));
    unsigned int a, b;
    a = LCD_ReadPoint(100, 100);
    b = LCD_ReadPoint(100, 101);
    printf("\n");
    printf("a = 0x%08X, b = 0x%08X\n", a, b);


    while (1)
    {

        
        
    }
}