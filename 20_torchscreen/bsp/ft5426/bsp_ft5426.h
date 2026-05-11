#ifndef __BSP_FT5426_H__
#define __BSP_FT5426_H__

#include "imx6ull.h"

#define FT5426_ADDRESS                      0X38     /* FT5426设备地址 */


/* 定义寄存器宏 */
#define FT5426_DEVICE_MODE 0X00 /* 模式寄存器 */
#define FT5426_IDGLIB_VERSION 0XA1 /* 固件版本寄存器 */
#define FT5426_IDG_MODE 0XA4 /* 中断模式 */
#define FT5426_TD_STATUS 0X02 /* 触摸状态寄存器 */
#define FT5426_TOUCH1_XH 0X03 /* 触摸点坐标寄存器,  一个触摸点用4个寄存器*/ 
#define FT5426_XYCOORDREG_NUM    30     /* 触摸点坐标寄存器数量 */
#define FT5426_INIT_FINISHED 1 /* 触摸屏初始化完成 */
#define FT5426_INIT_NOTFINISHED 0 /* 触摸屏初始化未完成 */
#define FT5426_TOUCH_EVENT_DOWN 0x00 /* 按下 */
#define FT5426_TOUCH_EVENT_UP 0x01 /* 释放 */
#define FT5426_TOUCH_EVENT_ON 0x02 /* 接触 */
#define FT5426_TOUCH_EVENT_RESERVED 0x03 /* 没有事件 */

/* 触摸屏结构体 */
typedef struct FT5426_Dev_struc
{
    unsigned char InitFalg; /* 触摸屏初始化状态 */
    unsigned char Intflag; /* 标记中断有没有发生 */
    unsigned char Point_num; /* 触摸点 */
    unsigned short x[5]; /* X轴坐标 */
    unsigned short y[5]; /* Y轴坐标 */
}FT5426_Dev_t;

extern FT5426_Dev_t FT5426_Dev;


void FT5426_Init(void);
void gpio_io9_irq_handler_t(unsigned int giccIar, void *param);
unsigned char FT5426_WriteByte(unsigned char add, unsigned char reg, unsigned char data);
unsigned char FT5426_ReadByte(unsigned char add, unsigned char reg);
void FT5426_Read_lenByte(unsigned char add, unsigned char reg, unsigned char len, unsigned char *buf);
void FT5426_ReadTouchPoint(void);

#endif