#include "bsp_ap3216c.h"
#include "bsp_i2c.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"
#include "stdio.h"

/* 初始化AP3216C */
void AP3216C_Init()
{
    unsigned char vlaue = 0;
    /* IO 初始化 */
    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1);  /* 复用IO为I2C_SCL 复用IO为I2C_SDA */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO05_GPIO1_IO05, 0x70B0);  /* 设置IO电器属性 */


    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1);  /* 复用IO为I2C_SDA 复用IO为I2C_SDA*/
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO05_GPIO1_IO05, 0x70B0);  /* 设置IO电器属性 */

    /* I2C接口初始化 */
    I2C_Init(I2C1);

    /* AP3216C传感器初始化 */
    AP3216C_WriteByte(AP3216C_ADDRESS, AP3216C_System_Configuration, 0x04); /* 使能传感器 */
    Delay_ms(100);
    AP3216C_WriteByte(AP3216C_ADDRESS, AP3216C_System_Configuration, 0x03);

    vlaue = AP3216C_ReadByte(AP3216C_ADDRESS, AP3216C_System_Configuration);
    printf("AP3216C System_Configuration: 0x%#02X\n", vlaue);
    
}

/* AP3216C读一个字节寄存器  返回值就是读取到的值 */
unsigned char AP3216C_ReadByte(unsigned char add, unsigned char reg)
{
    unsigned char vlaue = 0;

    I2C_Transfer I2C_Slave_Transfer;
    I2C_Slave_Transfer.SlaveAddress = add;  /* 从设备地址 */
    I2C_Slave_Transfer.Subaddress = reg;  /* 寄存器地址 */
    I2C_Slave_Transfer.Data = &vlaue;  /* 数据指针 */
    I2C_Slave_Transfer.DataSize = 1;  /* 数据大小 */
    I2C_Slave_Transfer.Direction = I2C_Read;  /* 读取方向 */
    I2C_Slave_Transfer.SubaddressSize = 1;  /* 寄存器地址长度 */
    I2C_Master_Transfer(I2C1, &I2C_Slave_Transfer);
    return vlaue;


}

/* AP3216C写一个字节寄存器  返回值就是写入的值 */
unsigned char AP3216C_WriteByte(unsigned char add, unsigned char reg, unsigned char data)
{
    unsigned char writedata = data;
    unsigned char status = 0;


    I2C_Transfer I2C_Slave_Transfer;
    I2C_Slave_Transfer.SlaveAddress = add;  /* 从设备地址 */
    I2C_Slave_Transfer.Subaddress = reg;  /* 寄存器地址 */
    I2C_Slave_Transfer.SubaddressSize = 1;  /* 寄存器地址长度 */
    I2C_Slave_Transfer.Data = &writedata;  /* 数据指针 */
    I2C_Slave_Transfer.DataSize = 1;  /* 数据大小 */
    I2C_Slave_Transfer.Direction = I2C_Write;  /* 写入方向 */
    if ((status = I2C_Master_Transfer(I2C1, &I2C_Slave_Transfer)) != I2C_STATUS_OK)
    {
        status = 1;
    }



    return status;


}



/* 读取AP3216C 传感器的数据 */
void AP3216C_ReadData(unsigned short *IR, unsigned short *PS, unsigned short *ALS)
{
    unsigned char buf[6];
    unsigned char i = 0;
    for (i = 0; i < 6; i++)
    {
        buf[i] = AP3216C_ReadByte(AP3216C_ADDRESS, AP3216C_IR_Data_Low + i);
    }
    
    if (buf[0] == 0x80) /* 为真 表示IR和PS数据无效 */
    {
        *IR = 0;
        *PS = 0;
    }
    else
    {
        *IR = ((unsigned short)buf[1] << 2) | (buf[0] & 0x03);
        *PS = (((unsigned short)buf[5] & 0x3F) << 4) | (buf[4] & 0x0F);
    }

    *ALS = ((unsigned short)buf[3] << 8 | buf[2]);
}