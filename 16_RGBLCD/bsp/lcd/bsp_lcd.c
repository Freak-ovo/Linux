#include "bsp_lcd.h"
#include "bsp_gpio.h"
#include "stdio.h"
#include "bsp_delay.h"

LCD_INFO_t TFT_LCD_DEV;

/* 初始化 */
void LCD_Init()
{
    unsigned short lcd_ID = 0;
    lcd_ID = LCD_Read_PanelID();
    printf("LCD_ID = %#X\r\n", lcd_ID);


    /* 初始化LCD_GPIO */
    LCD_GPIO_Init();
    /* 复位LCD控制器 */
    LCD_Reset();
    Delay_ms(100);
    /* 停止复位 */
    LCD_Reset_Stop();

    /* 根据不同的屏幕ID 来设置屏幕 */
    if (lcd_ID == ATK4342)
    {
        TFT_LCD_DEV.Height = 272;
        TFT_LCD_DEV.Width = 480;
        TFT_LCD_DEV.PixelSize = 1;
        TFT_LCD_DEV.VSPW = 1;
        TFT_LCD_DEV.VBPD = 8;
        TFT_LCD_DEV.VFPD = 8;
        TFT_LCD_DEV.HSPW = 1;
        TFT_LCD_DEV.HFPD = 5;
        TFT_LCD_DEV.HBPD = 40;
        LCD_PixelClock_Init(27, 8, 8);/* 设置10.1MHz */
        
        
    }
    TFT_LCD_DEV.PixelSize = 4;
    TFT_LCD_DEV.FrameBuffer = LCD_FRAME_ADDR;
    TFT_LCD_DEV.ForeColor = LCD_WHITE;
    TFT_LCD_DEV.BackColor = LCD_BLACK;
    
    /* 配置LCDIF接口 */
    LCDIF->CTRL = 0;    /* 清零 */

    LCDIF->CTRL |= (0 < 1) | (1 << 5) | (1 << 5) | (3 << 8) | (3 << 10) | (1 << 17) | (1 << 19);
    
    LCDIF->CTRL1 = 0;   /* 清零 */
    LCDIF->CTRL1 |= (7 << 16);

    LCDIF->TRANSFER_COUNT = 0;
    LCDIF->TRANSFER_COUNT |= (TFT_LCD_DEV.Height << 16) | (TFT_LCD_DEV.Width);

    LCDIF->VDCTRL0 = 0;
    LCDIF->VDCTRL0 |= (TFT_LCD_DEV.VSPW << 0) | (2 << 20) | (1 << 24) | (1 << 25) | (1 << 28);

    LCDIF->VDCTRL1 = TFT_LCD_DEV.VSPW + TFT_LCD_DEV.VBPD + TFT_LCD_DEV.Height + TFT_LCD_DEV.VFPD;
    LCDIF->VDCTRL2 = TFT_LCD_DEV.HSPW + TFT_LCD_DEV.HFPD + TFT_LCD_DEV.Width + TFT_LCD_DEV.HFPD  + (TFT_LCD_DEV.HSPW << 18);
    
    LCDIF->VDCTRL3 =(TFT_LCD_DEV.VSPW + TFT_LCD_DEV.VBPD) + ((TFT_LCD_DEV.HSPW + TFT_LCD_DEV.HBPD) << 16);

    LCDIF->VDCTRL4 = (TFT_LCD_DEV.Width) | (1 << 18);

    LCDIF->CUR_BUF = (unsigned int)TFT_LCD_DEV.FrameBuffer;

    LCDIF->NEXT_BUF = (unsigned int)TFT_LCD_DEV.FrameBuffer;

    LCD_Enable();

    Delay_ms(20);

    LCD_Clear(LCD_WHITE);
    
}

/* 像素时钟初始化 */
/***
 * @brief 像素时钟初始化
 * @param loopdiv 设置DIV_SELECT范围20-54
 * @param presdiv 预分数 设置1~8分频 
 * @param div 分数  可选范围1~8
 * LCDCLK = 24 * loopdiv / presdiv / div
 *
 */
void LCD_PixelClock_Init(unsigned char loopdiv, unsigned char presdiv, unsigned char div)
{
    /* 不使用小数分频 */
    CCM_ANALOG->PLL_VIDEO_NUM = 0;
    /* 设置DIV_SELECT */
    CCM_ANALOG->PLL_VIDEO_DENOM = 0;

    CCM_ANALOG->PLL_VIDEO |= (1 << 13) | (2 << 19) | (loopdiv << 0) | (presdiv << 4) | (div << 8);
    
    CCM_ANALOG->MISC2 &= ~(3 << 30);

    CCM->CSCDR2 &= ~(7 << 15);  /* 清零 */
    CCM->CSCDR2 |= (2 << 15);

    CCM->CSCDR2 &= ~(7 << 12);  /* 清零 */
    CCM->CSCDR2 |= ((presdiv - 1) << 12);

    CCM->CBCMR &= ~(7 << 23);    /* 清零 */
    CCM->CBCMR |= ((div - 1) << 23);

    CCM->CSCDR2 &= ~(7 << 9);   /* 清零 */
    CCM->CSCDR2 |= (0 << 9);
    
    

   }



/* 读取屏幕ID */
unsigned short LCD_Read_PanelID(void)
{
    unsigned int idx;
    /* 打开模拟开关，设置LCD_VSYNC为高电平 */
    gpio_pin_config_t lcd_gpio_pin_config;
    lcd_gpio_pin_config.direction = kGPIO_DigitalOutput;
    lcd_gpio_pin_config.outputlogic = 1;


    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);  /* 复用IO */
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0x10B0);  /* 设置IO电器属性 */

    /* GPIO初始化 */

    gpio_init(GPIO3, 3, &lcd_gpio_pin_config);

    /* 读取ID */

    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0);  /* 复用IO B7(M2) */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0);  /* 复用IO G7(M1) */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0);  /* 复用IO R7(M0) */

    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0X080);  /* 设置IO电器属性 */
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0X080);  /* 设置IO电器属性 */
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0X080);  /* 设置IO电器属性 */

    lcd_gpio_pin_config.direction = kGPIO_DigitalInput;
    
    gpio_init(GPIO3, 12, &lcd_gpio_pin_config);
    gpio_init(GPIO3, 20, &lcd_gpio_pin_config);
    gpio_init(GPIO3, 28, &lcd_gpio_pin_config);


    idx = (unsigned char)gpio_pinread(GPIO3, 28);
    idx |= (unsigned char)gpio_pinread(GPIO3, 20) << 1;
    idx |= (unsigned char)gpio_pinread(GPIO3, 28) << 2;

    switch (idx)
    {
        case 0:
        {
            return ATK4342;
        }
        case 4:
        {
            return ATK4384;
        }        
        case 1:
        {
            return ATK7084;
        }        
        case 2:
        {
            return ATK7016;
        }
        case 5:
        {
            return ATK1018;
        }
        default:
        {
            return 0;
        }
    }
}

void LCD_GPIO_Init(void)
{
    gpio_pin_config_t gpio_config;
    /* 1、IO初始化复用功能 */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0);

    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08,0);/* 背光引脚*/

    /* 2、配置LCD IO属性 
     *bit 16:0 HYS关闭 
     *bit [15:14]: 0 默认22K上拉
     *bit [13]: 0 pull功能
     *bit [12]: 0 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 111 驱动能力为R0/7
     *bit [0]: 1 高转换率 */ 
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00,0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01,0xB9);

    IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK,0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0xB9);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08,0xB9);
    /* GPIO初始化 */
    gpio_config.direction = kGPIO_DigitalOutput; /* 输出 */ 
    gpio_config.outputlogic = 1; /* 默认关闭背光 */ 
    gpio_init(GPIO1, 8, &gpio_config); /* 背光默认打开 */ 
    gpio_pinwrite(GPIO1, 8, 1);

}

/* 复位LCD控制器 */
void LCD_Reset(void)
{

    LCDIF->CTRL |= 1 << 31; /* 复位 */

}

/* 停止复位LCD控制器 */
void LCD_Reset_Stop(void)
{
    LCDIF->CTRL &= ~(1 << 31); /* 停止复位 */
}


/* 使能LCD控制器 */
void LCD_Enable(void)
{
    LCDIF->CTRL |= 1 << 0; /* 使能 */
}

#if 0
/* 画点函数 */
inline void LCD_DrawPoint(unsigned short x,unsigned short y, unsigned int color)
{
    *(unsigned int*)((unsigned int)TFT_LCD_DEV.FrameBuffer + TFT_LCD_DEV.PixelSize * (TFT_LCD_DEV.Width * y + x)) = color;
}

/* 读点函数 */
inline unsigned int LCD_ReadPoint(unsigned short x, unsigned short y)
{
    return *(unsigned int *)(unsigned int)TFT_LCD_DEV.FrameBuffer + TFT_LCD_DEV.PixelSize*(TFT_LCD_DEV.Width*y + x);

}
#endif

/* 清屏函数 */
void LCD_Clear(unsigned int color)
{
    unsigned int num;
    unsigned int i = 0;
    
    unsigned int *startaddr = (unsigned int *)LCD_FRAME_ADDR;

    num = (unsigned int)TFT_LCD_DEV.Width * TFT_LCD_DEV.Height;

    for (i = 0; i < num; i++)
    {
        startaddr[i] = color;
    }
}