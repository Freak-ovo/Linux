#ifndef __BSP_AP3216C_H__
#define __BSP_AP3216C_H__


#include "imx6ull.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

/* slave 地址 */
#define AP3216C_ADDRESS                     0x1E


/* 定义寄存器地址 */
#define AP3216C_System_Configuration        0x00
#define AP3216C_Interrupt_Status            0x01
#define AP3216C_INT_Clear_Manner            0x02
#define AP3216C_IR_Data_Low                 0x0A
#define AP3216C_IR_Data_High                0x0B
#define AP3216C_ALS_Data_Low                0x0C
#define AP3216C_ALS_Data_High               0x0D
#define AP3216C_PS_Data_Low                 0x0E
#define AP3216C_PS_Data_High                0x0F




void AP3216C_Init();
unsigned char AP3216C_ReadByte(unsigned char add, unsigned char reg);
unsigned char AP3216C_WriteByte(unsigned char add, unsigned char reg, unsigned char data);


void AP3216C_ReadData(unsigned short *IR, unsigned short *PS, unsigned short *ALS);


#endif
