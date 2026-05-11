#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "imx6ull.h"


void SPI_Init(ECSPI_Type *base);
unsigned char SPI_ReadWriteByte(ECSPI_Type *base, unsigned char Txdata);


#endif /* __BSP_SPI_H__ */