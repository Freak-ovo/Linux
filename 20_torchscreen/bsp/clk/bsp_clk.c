#include "bsp_clk.h"


/* 使能外设时钟 */

void CLK_Enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;

}

/* 初始化时钟 */
void imx6ull_clk_init(void)
{
    unsigned int reg;
    /* 初始化6ull的主频为528MHz */
    if (((CCM->CCSR >> 2) & 0x01) == 0x00)/* 当前时钟使用pll1_main_clk也就是PPL1 */
    {
        CCM->CCSR &= ~(1 << 8); /* 设置step_clk = osc_clk = 24MHz */
        CCM->CCSR |= (1 << 2); /* 设置pll1_sw_clk = step_clk = 24MHz */
        
    }
    /* 设置PLL1 = 1056MHz */
    CCM_ANALOG->PLL_ARM = (88 << 0) & 0x7F; /* 设置DIV_SELECT = 88 */
    CCM_ANALOG->PLL_ARM |= (1 << 13);   /* 使能PLL1 */
    CCM->CACRR = 1; /* 设置ARM_PODF = 2分频 */
    CCM->CCSR &= ~(1 << 2); /* 设置pll1_sw_clk = step_main_clk = 1056MHz */

    /* 设置PLL2的4路PFD时钟频率 */
    reg = CCM_ANALOG->PFD_528;  /* 获取PFD_528寄存器的值 */
    reg &= ~(0x3F3F3F3F);   /* 清除PFD0~3的位 */
    reg |= (32 << 24);  /* 设置PLL2_PFD03 = 297MHz*/
    reg |= (24 << 16);  /* 设置PLL2_PFD02 = 396MHz */
    reg |= (16 << 8);   /* 设置PLL2_PFD01 = 594MHz */
    reg |= (27 << 0);    /* 设置PLL2_PFD00 = 352MHz */
    CCM_ANALOG->PFD_528 = reg;  /* 写入PFD_528寄存器 */

    /* 设置PLL3的4路PFD时钟频率 */
    reg = CCM_ANALOG->PFD_480;  /* 获取PFD_480寄存器的值 */
    reg &= ~(0x3F3F3F3F);   /* 清除PFD0~3的位 */
    reg |= (19 << 24);  /* 设置PLL3_PFD03 = 454.7MHz*/
    reg |= (17 << 16);  /* 设置PLL3_PFD02 = 508.2MHz */
    reg |= (16 << 8);   /* 设置PLL3_PFD01 = 540MHz */
    reg |= (12 << 0);    /* 设置PLL3_PFD00 = 720MHz */  
    CCM_ANALOG->PFD_480 = reg;  /* 写入PFD_480寄存器 */

    /* 设置AHB_CLK_ROOT = 132MHz */
    CCM->CBCMR &= ~(3 << 18); /* 清零AHB_CLK_ROOT的位域 */
    CCM->CBCMR |= (1 << 18); /* 设置pre_periph_clk=PLL2_PFD2 = 396MHz */
    
    CCM->CBCMR &= ~(1 << 25); /* 清零PRE_PERIPH_CLK位 */
    CCM->CBCMR |= (1 << 25); /* 设置PRE_PERIPH_CLK位为1，使能pre_periph_clk */
    while (CCM->CDHIPR & (1 << 5)); /* 等待握手信号完成 */
    
    CCM->CBCDR &= ~(7 << 10); /* 清零AHB_PODF位的位域 */
    CCM->CBCDR |= (2 << 10); /* 设置AHB_PODF位为2，3分频 */
    while (CCM->CDHIPR & (1 << 1)); /* 等待握手信号完成 */

    /* 设置CBCDR[IPG_PODF] = 66MHz */
    CCM->CBCDR &= ~(3 << 8); /* 清零IPG_PODF位的位域 */
    CCM->CBCDR |= (1 << 8); /* 设置IPG_PODF位为1，2分频(132MHz/2 = 66MHz
    
    
    ) */

    /* PERCLK_CLK_ROOT */
    CCM->CBCDR &= ~(1<< 6); /* PERCLK_CLK_SEL时钟源选择IPG_CLK_ROOT */
    CCM->CBCDR &= ~(0x3F<< 6); /* 1分频,PERCLK_CLK_ROOT = 66MHz */


    /* 设置UART1_CLK_ROOT = 80MHz pll3_PFD02 */

    CCM->CSCMR1 &= ~(1 << 6); /* 选择UART1的时钟源为pll3_PFD02 */
    /* 设置UART1的时钟分频为1，串口时钟=pll3_PFD02/1 = 80MHz */
    CCM->CSCMR1 &= ~(0x3F << 0); /* 1分频 */


    /* 设置 ECSPI根时钟为PLL_60MHz */
    CCM->CSCDR2 &= ~(1 << 18);  /* ECSPI 时钟源为PLL3_60MHz */
    CCM->CSCDR2 &= ~(0x3F << 19);  /* 1分频 */



}