#ifndef BSP_LCDAPI_H
#define BSP_LCDAPI_H
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_lcdapi.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : LCD显示API函数。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/3/18 左忠凯创建
***************************************************************/
#include "imx6ull.h"
#include "bsp_lcd.h"

/* 函数声明 */
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void LCD_Draw_Circle(unsigned short x0,unsigned short y0,unsigned char r);
void LCD_ShowChar(unsigned     short x,unsigned short y,unsigned char num,unsigned char size, unsigned char mode);
unsigned int LCD_Pow(unsigned char m,unsigned char n);
void LCD_ShowNum(unsigned short x, unsigned short y, unsigned int num, unsigned char len,unsigned char size);
void LCD_ShowXNum(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size, unsigned char mode);
void LCD_ShowString(unsigned short x,unsigned short y,
unsigned short width, unsigned short height, unsigned char size,char *p);

#endif

