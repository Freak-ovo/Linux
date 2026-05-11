#include "bsp_icm20608.h"
#include "bsp_delay.h"
#include "bsp_spi.h"
#include <stdio.h>

ICM20608_dev_t ICM20608_dev;	/* ICM20608设备 */

/* 初始化 ICM20608 */
void ICM20608_Init()
{
    unsigned char regval = 0;

    /* SPI 引脚初始化 */

    IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 1);  /* 复用ECSPI3_CLK */
    IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 1);  /* 复用ECSPI3_MOSI */
    IOMUXC_SetPinMux(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 1);  /* 复用ECSPI3_MISO */


    IOMUXC_SetPinConfig(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0x10B0);  /* 设置IO电器属性 */
    IOMUXC_SetPinConfig(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0x10B0);  /* 设置IO电器属性 */
    IOMUXC_SetPinConfig(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0x10B0);  /* 设置IO电器属性 */

    /* 片选IO 初始化 */
    IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);  /* 复用ECSPI3_MISO */
    IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0x10B0);  /* 设置IO电器属性 */
    gpio_pin_config_t SPI_CS_config;
    SPI_CS_config.direction = kGPIO_DigitalOutput;
    SPI_CS_config.outputlogic = 0;
    gpio_init(GPIO1, 20, &SPI_CS_config);




    /* SPI控制器初始化 */
    SPI_Init(ECSPI3);
    

    /* ICM20608 */
    regval = ICM20608_ReadByte(ICM20_WHO_AM_I);
    printf("ICM20608 ID = 0x%02X\n", regval);

    ICM20608_WriteByte(ICM20_PWR_MGMT_1, 0x00);   /* 打开加速度计和陀螺仪所有轴*/
    Delay_ms(50);
    ICM20608_WriteByte(ICM20_PWR_MGMT_1, 0x01);   /* 打开加速度计和陀螺仪所有轴*/
    Delay_ms(50);





    ICM20608_WriteByte(ICM20_SMPLRT_DIV, 0x00); /* 输出采样率是内部采样率 */
    ICM20608_WriteByte(ICM20_GYRO_CONFIG, 0x18);   /* 陀螺仪±2000dps量程 */
    ICM20608_WriteByte(ICM20_ACCEL_CONFIG, 0x18);   /* 加速度计±16G量程 */
    ICM20608_WriteByte(ICM20_CONFIG, 0x04);   /* 陀螺仪的低通滤波器BW = 20Hz*/
    ICM20608_WriteByte(ICM20_ACCEL_CONFIG2, 0x04);   /* 加速度计的低通滤波器BW = 21.2Hz*/
    ICM20608_WriteByte(ICM20_PWR_MGMT_2, 0x00);   /* 打开加速度计和陀螺仪所有轴*/
    ICM20608_WriteByte(ICM20_LP_MODE_CFG, 0x00);   /* 关闭低功耗模式，保持正常工作模式*/
    ICM20608_WriteByte(ICM20_FIFO_EN, 0x00);   /* 关闭FIFO功能*/




}

/* ICM20608 通过SPI接口 读数据 */
unsigned char ICM20608_ReadByte(unsigned char reg)
{
    unsigned char reg_val = 0;
    reg |= 0x80;    /* 最高位置1，bit[7] = 1，表示读取数据 */
    ICM20608_CSN(0);    /* 片选拉低 */

    SPI_ReadWriteByte(ECSPI3, reg); /* 发送寄存器地址 */
    reg_val = SPI_ReadWriteByte(ECSPI3, 0XFF); /* 从机返回寄存器数据 */

    ICM20608_CSN(1);    /* 片选拉高 */

    return reg_val;
}


/* ICM20608 通过SPI接口 写数据 */
void ICM20608_WriteByte(unsigned char reg, unsigned char value)
{

    reg &= ~(0x80);    /* 最高位置1，bit[7] = 1，表示读取数据 */
    ICM20608_CSN(0);    /* 片选拉低 */

    SPI_ReadWriteByte(ECSPI3, reg); /* 发送寄存器地址 */
    SPI_ReadWriteByte(ECSPI3, value); /* 要写入的数据 */

    ICM20608_CSN(1);    /* 片选拉高 */

}


/* 读取寄存器 */
void ICM20608_Read_len(unsigned char reg, unsigned char *buf, unsigned char len)
{
    unsigned char i = 0;
    reg |= 0x80;    /* 最高位置1，bit[7] = 1，表示读取数据 */
    ICM20608_CSN(0);    /* 片选拉低 */

    SPI_ReadWriteByte(ECSPI3, reg); /* 发送寄存器地址 */
    for (i = 0; i < len; i++)
    {
        buf[i] = SPI_ReadWriteByte(ECSPI3, 0XFF); /* 从机返回寄存器数据 */
    }

    ICM20608_CSN(1);    /* 片选拉高 */


}

/* 获取陀螺仪分辨率 */
float ICM20608_gyro_Scaleget(void)
{
    unsigned char data;
    float gyro_scale = 0.0f;
    
    data = (ICM20608_ReadByte(ICM20_GYRO_CONFIG) >> 3) & 0x03;
    switch (data)
    {
    case 0:
        gyro_scale = 131;
        break;
    case 1:
        gyro_scale = 65.5;
        break;
    case 2:
        gyro_scale = 32.8;
        break;
    case 3:
        gyro_scale = 16.4;
        break;
    default:
        break;
    }



    return gyro_scale;



}


/* 获取加速度计分辨率 */
unsigned short ICM20608_accel_Scaleget(void)
{
    unsigned char data;
    unsigned short accel_scale = 0;
    
    data = (ICM20608_ReadByte(ICM20_ACCEL_CONFIG) >> 3) & 0x03;
    switch (data)
    {
    case 0:
        accel_scale = 16384;
        break;
    case 1:
        accel_scale = 8192;
        break;
    case 2:
        accel_scale = 4096;
        break;
    case 3:
        accel_scale = 2048;
        break;
    default:
        break;
    }



    return accel_scale;

}

/* 获取 ICM20608 内部数据 */
void ICM20608_Get_data()
{
    unsigned char data[14];
    ICM20608_Read_len(ICM20_ACCEL_XOUT_H, data, 14);


    ICM20608_dev.accel_x_adc = ((unsigned short)(data[0] << 8) | data[1]);
    ICM20608_dev.accel_y_adc = ((unsigned short)(data[2] << 8) | data[3]);
    ICM20608_dev.accel_z_adc = ((unsigned short)(data[4] << 8) | data[5]);
    ICM20608_dev.temp_adc = ((unsigned short)(data[6] << 8) | data[7]);

    ICM20608_dev.gyro_x_adc = ((unsigned short)(data[8] << 8) | data[9]);
    ICM20608_dev.gyro_y_adc = ((unsigned short)(data[10] << 8) | data[11]);
    ICM20608_dev.gyro_z_adc = ((unsigned short)(data[12] << 8) | data[13]);



    /* 计算实际数据，实际数据扩大100倍，0.01扩大到100倍=1 */
    ICM20608_dev.accel_x_act = ((float)ICM20608_dev.accel_x_adc / ICM20608_accel_Scaleget()) * 100;
    ICM20608_dev.accel_y_act = ((float)ICM20608_dev.accel_y_adc / ICM20608_accel_Scaleget()) * 100;
    ICM20608_dev.accel_z_act = ((float)ICM20608_dev.accel_z_adc / ICM20608_accel_Scaleget()) * 100;
    ICM20608_dev.temp_act = ((float)(ICM20608_dev.temp_adc - 25) / 326.8+25)*100;

    ICM20608_dev.gyro_x_act = ((float)ICM20608_dev.gyro_x_adc / ICM20608_gyro_Scaleget()) * 100;
    ICM20608_dev.gyro_y_act = ((float)ICM20608_dev.gyro_y_adc / ICM20608_gyro_Scaleget()) * 100;
    ICM20608_dev.gyro_z_act = ((float)ICM20608_dev.gyro_z_adc / ICM20608_gyro_Scaleget()) * 100;


}