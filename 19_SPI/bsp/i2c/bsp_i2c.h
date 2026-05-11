#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "imx6ull.h"

/* 状态码定义 */
#define I2C_STATUS_OK           0
#define I2C_STATUS_BUSY         1
#define I2C_STATUS_IDLE         2
#define I2C_STATUS_NAK          3
#define I2C_STATUS_ARBITRATION  4
#define I2C_STATUS_TIMEOUT      5
#define I2C_STATUS_ADDRNAK      6

/* 传输方向枚举 */
enum I2C_Direction
{
    I2C_Write = 0x00,
    I2C_Read  = 0x01,
};

/* 主机传输结构体 */
typedef struct I2C_Transfer_t
{
    unsigned char SlaveAddress;      /* 7位从机地址 */
    enum I2C_Direction Direction;    /* 传输方向 */
    unsigned int Subaddress;         /* 寄存器地址 */
    unsigned char SubaddressSize;    /* 寄存器地址长度 */
    unsigned char *volatile Data;    /* 数据缓冲区 */
    volatile unsigned int DataSize;  /* 数据缓冲区长度 */
}I2C_Transfer;

/* 函数声明 */
void I2C_Init(I2C_Type *base);
unsigned char I2C_Start(I2C_Type *base, unsigned char address, enum I2C_Direction direction);
unsigned char I2C_Master_Stop(I2C_Type *base);
unsigned char I2C_Master_Repeat_Start(I2C_Type *base, unsigned char address, enum I2C_Direction direction);
unsigned char I2C_Check_And_Clear_Error(I2C_Type *base, unsigned int status);
void I2C_Master_Write(I2C_Type *base, const unsigned char *buf, unsigned int size);
void I2C_Master_Read(I2C_Type *base, unsigned char *buf, unsigned int size);
unsigned char I2C_Master_Transfer(I2C_Type *base, I2C_Transfer *xfer);

#endif