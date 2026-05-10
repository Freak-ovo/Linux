/**************************************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名    : led.s
作者      : 左忠凯
版本      : V1.0
描述      : 裸机实验 1 汇编点灯
            使用汇编来点亮开发板上的 LED 灯，学习和掌握如何用汇编语言来完成对 I.MX6U 处理器的 GPIO 初始化和控制。
其他      : 无
论坛      : www.openedv.com
日志      : 初版 V1.0 2019/1/3 左忠凯创建
**************************************************************************************/

.global _start          /* 全局标号 */

/*
 * 描述: _start 函数，程序从此函数开始执行此函数完成时钟使能、
 *       GPIO 初始化、最终控制 GPIO 输出低电平来点亮 LED 灯。
 */
_start:
    /* 1、使能所有时钟 */
    ldr r0, =0X020C4068   /* 寄存器 CCGR0 */
    ldr r1, =0XFFFFFFFF
    str r1, [r0]

    ldr r0, =0X020C406C   /* 寄存器 CCGR1 */
    str r1, [r0]

    ldr r0, =0X020C4070   /* 寄存器 CCGR2 */
    str r1, [r0]

    ldr r0, =0X020C4074   /* 寄存器 CCGR3 */
    str r1, [r0]

    ldr r0, =0X020C4078   /* 寄存器 CCGR4 */
    str r1, [r0]

    ldr r0, =0X020C407C   /* 寄存器 CCGR5 */
    str r1, [r0]

    ldr r0, =0X020C4080   /* 寄存器 CCGR6 */
    str r1, [r0]

    /* 2、设置 GPIO1_IO03 复用为 GPIO1_IO03 */
    ldr r0, =0X020E0068   /* 寄存器 SW_MUX_GPIO1_IO03_BASE */
    ldr r1, =0X05         /* 设置 MUX_MODE 为 5 (GPIO模式) */
    str r1, [r0]

    /* 3、配置 GPIO1_IO03 的 IO 属性 */
    /* bit 16:  0 关闭 HYS
     * bit[15:14]: 00 默认下拉
     * bit[13]:    0 keeper 功能
     * bit[12]:    1 pull/keeper 使能
     * bit[11]:    0 关闭开路输出
     * bit[7:6]:   10 速度 100MHz
     * bit[5:3]:   110 R0/6 驱动能力
     * bit[0]:     0 低转换率
     */
    ldr r0, =0X020E02F4   /* 寄存器 SW_PAD_GPIO1_IO03_BASE */
    ldr r1, =0X10B0
    str r1, [r0]

    /* 4、设置 GPIO1_IO03 为输出 */
    ldr r0, =0X0209C004   /* 寄存器 GPIO1_GDIR */
    ldr r1, =0X00000008   /* bit3 置1，设置为输出 */
    str r1, [r0]

    /* 5、打开 LED0：设置 GPIO1_IO03 输出低电平 */
    ldr r0, =0X0209C000   /* 寄存器 GPIO1_DR */
    ldr r1, =0X00000000   /* 输出低电平 —— 已补全 */
    str r1, [r0]

    /*
     * 描述: loop 死循环
     */
loop:
    b loop

    