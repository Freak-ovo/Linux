#include "bsp_spi.h"



/* SPI 初始化 */
void SPI_Init(ECSPI_Type *base)
{
    base->CONREG = 0;   /* 清零 */
    base->CONREG |= (1 << 0) | (1 << 3) | (1 << 4) | (7 << 20);

    base->CONFIGREG  = 0;

    base->PERIODREG = 0x2000;

    /* SPI 时钟 ICM20608的SPI最高到8MHz，将SPI CLK = 6MHz*/
    base->CONREG &= ~((0XF << 12) | (0XF << 8));    /* 先将bit[15:12]和bit[11:8] */
    base->CONREG |= (9 << 12);  /* 一级10分频  60MHz/10 = 6MHz*/








}

/* 通过SPI读写外设 */
unsigned char SPI_ReadWriteByte(ECSPI_Type *base, unsigned char Txdata)
{
    uint32_t SPI_RXDATA = 0;
    uint32_t SPI_TXDATA = Txdata;
    
    /* 选择通道ss0 */
    base->CONREG &= ~(3 << 18);
    base->CONREG |= (0 << 18);  /* 选择通道0 */

    /* 数据发送 */
    while ((base->STATREG & (1 << 0)) == 0);
    base->TXDATA = SPI_TXDATA;

    /* 接收数据 */
    while ((base->STATREG & (1 << 3)) == 0);
    SPI_RXDATA = base->RXDATA;

    return SPI_RXDATA;


}