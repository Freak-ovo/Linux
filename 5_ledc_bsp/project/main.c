#include "fsl_common.h"    /* 引入NXP官方SDK的通用宏定义和基础类型 */
#include "fsl_iomuxc.h"    /* 引入IOMUXC（引脚复用）相关的寄存器配置函数 */
#include "MCIMX6Y2.h"      /* 引入I.MX6ULL芯片的核心寄存器映射头文件 */

#include "bsp_clk.h"       /* 引入自定义的板级时钟驱动头文件 */
#include "bsp_delay.h"     /* 引入自定义的板级延时驱动头文件 */
#include "bsp_led.h"       /* 引入自定义的板级LED驱动头文件 */

/* 程序主入口，裸机程序的起点 */
int main(void)
{
    /* 第一步：使能开发板上所有外设的时钟信号（类似于给所有硬件模块通电） */
    CLK_Enable(); 
    
    /* 第二步：初始化LED灯对应的GPIO引脚（配置引脚复用功能、电气属性及输出方向） */
    LED_Init(); 

    /* 进入主程序的无限循环，让开发板持续运行 */
    while (1)
    {
        /* 第三步：调用LED驱动函数，点亮LED0（传入参数1代表开启/ON状态） */
        LED_Switch(LED0, 1); 
        Delay(500);          /* 调用延时函数，保持点亮状态约500毫秒 */
        
        /* 第四步：调用LED驱动函数，熄灭LED0（传入参数0代表关闭/OFF状态） */
        LED_Switch(LED0, 0);  
        Delay(500);          /* 调用延时函数，保持熄灭状态约500毫秒 */
    }
    return 0; /* 由于程序在while(1)中死循环，实际上永远不会执行到这一行 */
}