#ifndef _BSP_LCD_H
#define _BSP_LCD_H


#include "imx6ull.h"

/* LCD显存首地址 */
#define LCD_FRAME_ADDR 0x89000000

/* LCD屏幕信息结构体 */
typedef struct TFT_LCD_INFO_t
{
    unsigned short Height;  /* 屏幕高度 */
    unsigned short Width;   /* 屏幕宽度 */
    unsigned char PixelSize; /* 像素大小 */
    unsigned short VSPW;    /* 垂直同步宽度 */
    unsigned short VBPD;    /* 水平同步宽度 */
    unsigned short VFPD;    /* 垂直前 porch宽度 */
    unsigned short HSPW;    /* 水平同步宽度 */
    unsigned short HFPD;    /* 水平前 porch宽度 */
    unsigned short HBPD;    /* 水平前 porch宽度 */


    unsigned int FrameBuffer; /* 屏幕显存起始地址 */
    unsigned int ForeColor;   /* 前景色颜色值 */
    unsigned int BackColor;   /* 背景色颜色值 */

} LCD_INFO_t;


/* 屏幕参数结构体变量 */
extern LCD_INFO_t TFT_LCD_DEV;

/* LCD颜色定义 */
#define LCD_BLUE 0x000000FF                 /* 蓝色 */
#define LCD_GREEN 0x0000FF00               /* 绿色 */
#define LCD_RED 0x00FF0000                 /* 红色 */
#define LCD_WHITE 0x00FF00FF               /* 白色 */
#define LCD_BLACK 0x000000                 /* 黑色 */
#define LCD_YELLOW 0xFF0000               /* 黄色 */
#define LCD_MAGENTA 0xFF00FF               /* 紫色 */
#define LCD_CYAN 0x00FF00                 /* 青色 */
#define LCD_GRAY 0x808080                 /* 灰色 */
#define LCD_TRANSPARENT 0x00000000       /* 透明 */





/* 屏幕ID */
#define ATK4342     0x4342  /* 4.3寸480*272 */
#define ATK4384     0x4384  /* 4.3寸800*480 */
#define ATK7084     0x7084  /* 7 寸800*400 */
#define ATK7016     0x7016  /* 7寸1024*600 */
#define ATK1018     0x1018  /* 10.1寸1280*800 */







void LCD_Init();
unsigned short LCD_Read_PanelID(void);
void LCD_PixelClock_Init(unsigned char loopdiv, unsigned char presdiv, unsigned char div);
void LCD_GPIO_Init(void);

void LCD_Reset(void);
void LCD_Reset_Stop(void);
void LCD_Enable(void);




inline void LCD_DrawPoint(unsigned short x,unsigned short y, unsigned int color)
{
    *(unsigned int*)((unsigned int)TFT_LCD_DEV.FrameBuffer + TFT_LCD_DEV.PixelSize * (TFT_LCD_DEV.Width * y + x)) = color;
}

inline unsigned int LCD_ReadPoint(unsigned short x, unsigned short y)
{
    return *(unsigned int *)((unsigned int)TFT_LCD_DEV.FrameBuffer + TFT_LCD_DEV.PixelSize * (TFT_LCD_DEV.Width * y + x));

}



void LCD_Clear(unsigned int color);

#endif
