#include "bsp_ft5426.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_delay.h"
#include "bsp_i2c.h"
#include "stdio.h"




FT5426_Dev_t FT5426_Dev;


/* 初始化 */
void FT5426_Init(void)
{
    unsigned char reg_vlaue[2];
    /* IO 初始化 */
    IOMUXC_SetPinMux(IOMUXC_UART5_TX_DATA_I2C2_SCL, 1);  /* 复用I2C2_CLK */
    IOMUXC_SetPinMux(IOMUXC_UART5_RX_DATA_I2C2_SDA, 1);  /* 复用I2C2_SDA */

    IOMUXC_SetPinConfig(IOMUXC_UART5_TX_DATA_I2C2_SCL, 0x70B0);  /* 设置IO电器属性 */
    IOMUXC_SetPinConfig(IOMUXC_UART5_RX_DATA_I2C2_SDA, 0x70B0);  /* 设置IO电器属性 */

    /* 复位IO 输出io */
    /* 初始化GPIO1_IO09为输出模式 */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 1);  /* 复用IO */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0xF080);  /* 设置IO电器属性 */

    gpio_pin_config_t reset_pin_config;
    reset_pin_config.direction = kGPIO_IntRisingOrFallEdge; /* 上升或下降沿触发 */
    reset_pin_config.outputlogic = 1;
    gpio_init(GPIO1, 9, &reset_pin_config);


    /* 使能中断 */
    sys_irq_handler_t   ctint_io9_handler;
    ctint_io9_handler.handler = gpio_io9_irq_handler_t;
    ctint_io9_handler.param = NULL;

    /* 使能中断 */
    GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
    /* 注册中断服务函数 */
    system_register_irq_handler(GPIO1_Combined_0_15_IRQn, ctint_io9_handler.handler, NULL);
    /* 使能GPIO9中断 */
    gpio_enable_interrupt(GPIO1, 9);

    /* 初始化 RST 引脚使能中断*/
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 1);  /* 复用IO */
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0x10B0);  /* 设置IO电器属性 */

    gpio_pin_config_t ctrst_pin_config;
    ctrst_pin_config.direction = kGPIO_IntRisingOrFallEdge; /* 上升或下降沿触发 */
    ctrst_pin_config.outputlogic = 1;
    gpio_init(GPIO5, 9, &ctrst_pin_config);

    /* 复位GPIO */
    gpio_pinwrite(GPIO5, 9, 0); /* 复位低电平 */
    Delay_ms(50);
    gpio_pinwrite(GPIO5, 9, 1); /* 定制复位 */
    Delay_ms(50);


    /* I2C 初始化 */
    I2C_Init(I2C2);


    /* FT5426 初始化 */
    FT5426_Read_lenByte(FT5426_ADDRESS, FT5426_IDGLIB_VERSION, 2, reg_vlaue);
    printf("touch FrimWare Version: %#X\r\n", ((unsigned char)(reg_vlaue[0] << 8) | reg_vlaue[1]));


    FT5426_WriteByte(FT5426_ADDRESS, FT5426_DEVICE_MODE, 0);    /* 设置工作模式 */

    FT5426_WriteByte(FT5426_ADDRESS,FT5426_IDG_MODE, 1);    /* 设置中断模式 */

    /* 初始化完成标志位 */
    FT5426_Dev.InitFalg  = FT5426_INIT_FINISHED;



}


/* 外部中断服务函数 */
void gpio_io9_irq_handler_t(unsigned int giccIar, void *param)
{
    //static unsigned char state = 0;
    if (FT5426_Dev.InitFalg != FT5426_INIT_FINISHED)
    {
        return;
    }
    printf("Touch int \r\n");

    /* 清楚中断标志位 */
    gpio_clear_interrupt_flag(GPIO1, 9);



}

/* 向FT5426写一个字节的数据 */
unsigned char FT5426_WriteByte(unsigned char add, unsigned char reg, unsigned char data)
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
    if ((status = I2C_Master_Transfer(I2C2, &I2C_Slave_Transfer)) != I2C_STATUS_OK)
    {
        status = 1;
    }

    return status;


}

/* 从FT5426读取一个字节数据 */
unsigned char FT5426_ReadByte(unsigned char add, unsigned char reg)
{
    unsigned char vlaue = 0;

    I2C_Transfer I2C_Slave_Transfer;
    I2C_Slave_Transfer.SlaveAddress = add;  /* 从设备地址 */
    I2C_Slave_Transfer.Subaddress = reg;  /* 寄存器地址 */
    I2C_Slave_Transfer.Data = &vlaue;  /* 数据指针 */
    I2C_Slave_Transfer.DataSize = 1;  /* 数据大小 */
    I2C_Slave_Transfer.Direction = I2C_Read;  /* 读取方向 */
    I2C_Slave_Transfer.SubaddressSize = 1;  /* 寄存器地址长度 */
    I2C_Master_Transfer(I2C2, &I2C_Slave_Transfer);
    return vlaue;


}
/* 从FT5426 读取多个寄存器数据 */
void FT5426_Read_lenByte(unsigned char add, unsigned char reg, unsigned char len, unsigned char *buf)
{


    I2C_Transfer I2C_Slave_Transfer;
    I2C_Slave_Transfer.SlaveAddress = add;  /* 从设备地址 */
    I2C_Slave_Transfer.Subaddress = reg;  /* 寄存器地址 */
    I2C_Slave_Transfer.DataSize = 1;  /* 数据大小 */
    I2C_Slave_Transfer.Direction = I2C_Read;  /* 读取方向 */
    I2C_Slave_Transfer.SubaddressSize = len;  /* 寄存器地址长度 */
    I2C_Slave_Transfer.Data = buf;  /* 数据指针 */
    I2C_Master_Transfer(I2C2, &I2C_Slave_Transfer);



}


/* 读取触摸点数据 */
void FT5426_ReadTouchPoint(void)
{
    unsigned char i;
    unsigned char type = 0;
    unsigned char PointBuf[FT5426_XYCOORDREG_NUM];
    FT5426_Dev.Point_num = FT5426_ReadByte(FT5426_ADDRESS, FT5426_TD_STATUS);
    printf("Point_num: %d\r\n", FT5426_Dev.Point_num);

    /* 读取触摸点寄存器信息 */
    FT5426_Read_lenByte(FT5426_ADDRESS, FT5426_TOUCH1_XH, FT5426_XYCOORDREG_NUM, PointBuf);
    printf("Touch1_XH: %#X, Touch1_XL: %#X, Touch1_YH: %#X, Touch1_YL: %#X\r\n", PointBuf[0], PointBuf[1], PointBuf[2], PointBuf[3]);

    for (i = 0; i < FT5426_Dev.Point_num; i++)
    {
        unsigned char *buf = &PointBuf[i * 6];
        FT5426_Dev.x[i] = ((buf[0] << 8) | buf[1]) & 0xFFFF;    /* 触摸点X坐标寄存器值 */
        FT5426_Dev.y[i] = ((buf[2] << 8) | buf[3]) & 0xFFFF;     /* 触摸点Y坐标寄存器值 */
        printf("Touch%d_X: %#X, Touch%d_Y: %#X\r\n", i, FT5426_Dev.x[i], i, FT5426_Dev.y[i]);
        
        type = buf[0] >> 6;
        if (type == FT5426_TOUCH_EVENT_DOWN)
        {
            printf("Touch%d_Type: %#X\r\n", i, type);
        }
        else if (type == FT5426_TOUCH_EVENT_UP)
        {
            printf("Touch%d_Type: %#X\r\n", i, type);
        }
        else if (type == FT5426_TOUCH_EVENT_ON)
        {
            printf("Touch%d_Type: %#X\r\n", i, type);
        }
    }

}
