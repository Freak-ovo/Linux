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
#include "bsp_ft5426.h"

/* 
 * @description : 指定的位置显示整数数据
 * @param - x : X轴位置
 * @param - y : Y轴位置
 * @param - size: 字体大小
 * @param - num : 要显示的数据
 * @return : 无
 */
void integer_display(unsigned short x, unsigned short y, unsigned char size, signed int num)
{
    char buf[200];
    
    // 清除显示区域
    LCD_Clear(LCD_WHITE);
    
    memset(buf, 0, sizeof(buf));
    if(num < 0)
        sprintf(buf, "-%d", -num);
    else
        sprintf(buf, "%d", num);
    
    LCD_ShowString(x, y, 50, size, size, buf);
}

/* 
 * @description : 指定的位置显示小数数据,比如5123，显示为51.23
 * @param - x : X轴位置
 * @param - y : Y轴位置
 * @param - size: 字体大小
 * @param - num : 要显示的数据，实际小数扩大100倍
 * @return : 无
 */
void decimals_display(unsigned short x, unsigned short y, unsigned char size, signed int num)
{
    signed int integ; /* 整数部分 */
    signed int fract; /* 小数部分 */
    signed int uncomptemp = num;
    char buf[200];

    if(num < 0)
        uncomptemp = -uncomptemp;
    
    integ = uncomptemp / 100;
    fract = uncomptemp % 100;

    memset(buf, 0, sizeof(buf));
    if(num < 0)
        sprintf(buf, "-%d.%d", integ, fract);
    else
        sprintf(buf, "%d.%d", integ, fract);
    
    LCD_Clear(LCD_WHITE);
    LCD_ShowString(x, y, 60, size, size, buf);
}

/* 
 * @description : 使能I.MX6U的硬件NEON和FPU
 * @param : 无
 * @return : 无
 */
void imx6ul_hardfpu_enable(void)
{
    uint32_t cpacr;
    uint32_t fpexc;

    /* 使能NEON和FPU */
    cpacr = __get_CPACR();
    cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk)) 
            | (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
    __set_CPACR(cpacr);
    
    fpexc = __get_FPEXC();
    fpexc |= 0x40000000UL;
    __set_FPEXC(fpexc);
}

/* 程序主入口，裸机程序的起点 */
int main(void)
{
    // 启用硬件浮点运算单元
    imx6ul_hardfpu_enable(); 
    
    INT_Init();        
    CLK_Enable(); 
    UART_Init();
    LED_Init();
    Beep_Init();
    Key_Init();
    GIC_Init();
    Exit_Init();
    EPIT1_Init(0, 33000000); /* 500ms 周期的定时器 */
    KeyFilter_Init();
    LCD_Init();
    RTC_Init();
    AP3216C_Init();
    ICM20608_Init();
    FT5426_Init();

    TFT_LCD_DEV.ForeColor = LCD_RED;
    TFT_LCD_DEV.BackColor = LCD_BLACK;

    // 清屏
    LCD_Clear(LCD_WHITE);

    // 显示标题信息
    LCD_ShowString(10, 10, TFT_LCD_DEV.Width-10, 24, 24, (char *)"IMX6U-ALPHA SPI TEST");
    LCD_ShowString(10, 35, TFT_LCD_DEV.Width-10, 16, 16, (char *)"ICM20608 SENSOR DATA");
    
    // 检查ICM20608是否在线
    unsigned char regval = ICM20608_ReadByte(ICM20_WHO_AM_I);
    if (regval != 0xAB) // ICM20608的设备ID通常是0xAB
    {
        LCD_ShowString(10, 60, TFT_LCD_DEV.Width-10, 16, 16, (char *)"ICM20608 Check Failed!");
        while(1)
        {
            LED_Switch(LED0, !gpio_pinread(GPIO1, 3)); // LED闪烁报警
            Delay_ms(200);
        }
    }
    else
    {
        LCD_ShowString(10, 60, TFT_LCD_DEV.Width-10, 16, 16, (char *)"ICM20608 Ready");
    }

    // 显示数据标签
    LCD_ShowString(10, 90, 200, 16, 16, (char *)"IMX6ULL TOUCH TEST:");
    LCD_ShowString(10, 110, 200, 16, 16, (char *)"FT5426 TOUCH TEST:");
    LCD_ShowString(10, 130, 200, 16, 16, (char *)"2026-05-22");


    TFT_LCD_DEV.ForeColor = LCD_BLUE;

    bool status = false;
    while (1)
    {
        printf("Hello World!\n");
        printf("x: %d, y: %d\n", FT5426_Dev.x[0], FT5426_Dev.y[0]);
        status = !status;
        LED_Switch(LED0, status);
    }
    return 0;
}