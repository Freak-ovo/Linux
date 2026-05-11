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
#include "bsp_lcdapi.h"
#include "stdio.h"
#include "bsp_rtc.h"
#include "bsp_ap3216c.h"
#include "bsp_i2c.h"
#include "bsp_icm20608.h"
#include "bsp_spi.h"



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
    RTC_Init();
    AP3216C_Init();
    ICM20608_Init();

    TFT_LCD_DEV.ForeColor = LCD_RED;
    TFT_LCD_DEV.BackColor = LCD_BLACK;

    LCD_ShowString(10,40,260,32,32,(char *)"SPI:");

    LCD_ShowString(40,40,260,32,32,(char *)"ICM20608:");

    bool status = false;
    while (1)
    {
        unsigned char regval = ICM20608_ReadByte(ICM20_WHO_AM_I);
        printf("ICM20608 ID = 0x%02X\n", regval);
        LCD_ShowString(40,40,260,32,32,(char *)"ICM20608:");
        ICM20608_Get_data();
        printf("ICM20608 gyro_x_adc = %d, gyro_y_adc = %d, gyro_z_adc = %d, accel_x_adc = %d, accel_y_adc = %d, accel_z_adc = %d, temp_adc = %d\n", ICM20608_dev.gyro_x_adc, ICM20608_dev.gyro_y_adc, ICM20608_dev.gyro_z_adc, ICM20608_dev.accel_x_adc, ICM20608_dev.accel_y_adc, ICM20608_dev.accel_z_adc, ICM20608_dev.temp_adc);



        status = !status;
        LED_Switch(LED0, status);
        Delay_ms(500);
    }
}