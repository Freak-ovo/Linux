#include "bsp_i2c.h"

/* 
 * 函数名：I2C_Init
 * 描述  ：初始化 I2C 外设，设置时钟频率
 * 参数  ：base - I2C 寄存器基地址
 * 返回值：无
 */
void I2C_Init(I2C_Type *base)
{
    /* 1. 关闭 I2C 模块，以便配置 */
    base->I2CR &= ~(1 << 7); 
    
    /* 2. 设置波特率分频器 IFDR
     * 0x15 (二进制 0001 0101) 对应 640 分频
     * 假设内核时钟 660MHz: 660000000 / 660 = 1000000 (1MHz) -> 再经过内部机制通常得到 100kHz
     * 具体计算需参考芯片手册，此处为常见配置 */
    base->IFDR = 0x15;  

    /* 3. 使能 I2C 模块 */
    base->I2CR |= (1 << 7);     
}

/* 
 * 函数名：I2C_Start
 * 描述  ：产生 I2C Start 信号，并发送从机地址
 * 参数  ：base - 寄存器基地址
 *         address - 从机7位地址
 *         direction - 传输方向 (读/写)
 * 返回值：状态码
 */
unsigned char I2C_Start(I2C_Type *base, unsigned char address, enum I2C_Direction direction)
{
    /* 检查 I2C 是否忙 */
    if (base->I2SR & (1 << 5))  
        return I2C_STATUS_BUSY;

    /* 设置为主机模式 (MEN=1, MSTA=1) */
    base->I2CR |= (1 << 7) | (1 << 5); 

    /* 
     * 产生 Start 信号并发送地址
     * 注意：这里修正了运算符优先级问题，增加了括号
     */
    base->I2DR = ((unsigned int)(address << 1) | ((direction == I2C_Read) ? 1 : 0));
    
    return I2C_STATUS_OK;
}

/* 
 * 函数名：I2C_Master_Stop
 * 描述  ：产生 I2C Stop 信号
 * 参数  ：base - 寄存器基地址
 * 返回值：状态码 (超时或成功)
 */
unsigned char I2C_Master_Stop(I2C_Type *base)
{
    unsigned short timeout = 0xffff;

    /* 清除重复起始位和发送使能位，这将自动产生 Stop 信号 */
    base->I2CR &= ~((1 << 5) | (1 << 4) | (1 << 3));

    /* 等待 I2C 空闲 */
    while (base->I2SR & (1 << 5))
    {
        timeout--;
        if (timeout == 0)    
        {
            return I2C_STATUS_TIMEOUT;
        }
    }
    return I2C_STATUS_OK;
}

/* 
 * 函数名：I2C_Master_Repeat_Start
 * 描述  ：产生 I2C Repeated Start 信号
 * 参数  ：base - 寄存器基地址
 *         address - 从机7位地址
 *         direction - 传输方向
 * 返回值：0 (失败) 或 1 (成功，此处原书代码逻辑较简单)
 */
unsigned char I2C_Master_Repeat_Start(I2C_Type *base, unsigned char address, enum I2C_Direction direction)
{
    /* 检查总线是否忙或是否为主机模式 */
    if ((base->I2SR & (1 << 5)) || (!(base->I2CR & (1 << 5))) )
    {
        return 0; // 失败
    }
    
    /* 设置重复起始标志 */
    base->I2CR |= (1 << 4) | (1 << 2); 
    
    /* 发送器件地址 */
    base->I2DR = ((unsigned int)(address << 1) | ((direction == I2C_Read) ? 1 : 0));
    return 1; // 成功
}

/* 
 * 函数名：I2C_Check_And_Clear_Error
 * 描述  ：检查并清除 I2C 错误标志
 * 参数  ：base - 寄存器基地址, status - 状态寄存器值
 * 返回值：错误类型
 */
unsigned char I2C_Check_And_Clear_Error(I2C_Type *base, unsigned int status)
{
    /* 检查是否为仲裁丢失错误 */
    if (status & (1 << 4))
    {
        base->I2SR &= ~(1 << 4);       /* 清除仲裁丢失标志 */
        base->I2CR &= ~(1 << 7);       /* 关闭 I2C */
        base->I2CR |= (1 << 7);        /* 打开 I2C */
        return I2C_STATUS_ARBITRATION; /* 返回仲裁错误 */
    }
    else if (status & (1 << 0)) /* 检查是否为 NAK (非应答) */
    {
        return I2C_STATUS_NAK;         /* 返回非应答错误 */
    }
    return I2C_STATUS_OK;
}

/* 
 * 函数名：I2C_Master_Write
 * 描述  ：I2C 主机发送数据
 * 参数  ：base - 寄存器基地址, buf - 数据缓冲区, size - 数据长度
 * 返回值：无
 */
void I2C_Master_Write(I2C_Type *base, const unsigned char *buf, unsigned int size)
{
    /* 等待传输完成 (TCF 标志) */
    while(!(base->I2SR & (1 << 7)));

    /* 清除中断标志位，并准备发送 */
    base->I2SR &= ~(1 << 0); 
    base->I2SR |= (1 << 7);  /* 写1清0机制 */

    while (size--)
    {
        base->I2DR = *buf++;    /* 将数据写入数据寄存器，自动触发发送 */
        
        /* 等待一字节传输完成 */
        while (!(base->I2SR & (1 << 1))); 
        
        /* 清除中断标志 */
        base->I2SR &= ~(1 << 1); 

        /* 检查是否有错误 (如从机未应答) */
        if (I2C_Check_And_Clear_Error(base, base->I2SR))
            break; /* 如果出错，停止发送 */
    }

    /* 清除状态 */
    base->I2SR &= ~(1 << 1);
}

/* 
 * 函数名：I2C_Master_Read
 * 描述  ：I2C 主机接收数据
 * 参数  ：base - 寄存器基地址, buf - 数据缓冲区, size - 数据长度
 * 返回值：无
 */
void I2C_Master_Read(I2C_Type *base, unsigned char *buf, unsigned int size)
{
    volatile uint8_t dummy = 0; 
    dummy++; /* 假读操作，用于释放总线 */

    /* 等待状态完成 */
    while(!(base->I2SR & (1 << 7)));

    base->I2SR &= ~(1 << 1); /* 清除中断标志 */
    
    /* 设置接收模式：清除 TX (发送使能) 位，保持 MSTA (主机) 位 */
    base->I2CR &= ~(1 << 4); 
    base->I2CR |= (1 << 3);  /* 置位 TXAK (发送非应答)，默认为ACK */

    /* 根据数据长度决定是否在最后一个字节发送 NACK */
    if (size == 1)
    {
        base->I2CR |= (1 << 3); /* 第一个也是最后一个字节，发送 NACK */

        dummy = base->I2DR; /* 读取 I2DR 释放时钟 */

    }
    
    while (size--)
    {
        /* 等待接收完成 */
        while (!(base->I2SR & (1 << 1))); 
        base->I2SR &= ~(1 << 1); /* 清除中断 */

        if (size == 0)  
        {
            /* 最后一个字节，发送 Stop 信号 */
            I2C_Master_Stop(base);
        }
        else if (size == 1)    
        {
            /* 倒数第二个字节，需要设置为 NACK，为最后一个字节做准备 */
            base->I2CR |= (1 << 3); 
        }
        
        /* 读取数据 (读取 I2DR 会释放时钟) */
        *buf++ = base->I2DR;
    }
}

/* 
 * 函数名：I2C_Master_Transfer
 * 描述  ：I2C 通用传输函数 (读写寄存器的标准流程)
 * 参数  ：base - 寄存器基地址, xfer - 传输结构体
 * 返回值：状态码
 */
unsigned char I2C_Master_Transfer(I2C_Type *base, I2C_Transfer *xfer)
{
    unsigned char ret = 0;
    enum I2C_Direction direction = xfer->Direction;

    /* 清除状态寄存器标志 */
    base->I2SR &= ~((1 << 1) | (1 << 4)); 
    
    /* 等待传输完成 */
    while(!(base->I2SR & (1 << 7)));

    /* 如果是读操作且有子地址，先进行写操作发送地址 */
    if ((xfer->SubaddressSize > 0) && (xfer->Direction == I2C_Read))
        direction = I2C_Write;

    /* 发送 Start 和从机地址 */
    ret = I2C_Start(base, xfer->SlaveAddress, direction);
    if(ret) return ret;

    while(!(base->I2SR & (1 << 1))); /* 等待 */
    
    /* 检查地址是否被应答 */
    ret = I2C_Check_And_Clear_Error(base, base->I2SR);
    if(ret)
    {
        I2C_Master_Stop(base); 
        return ret;
    } 
    
    /* 发送寄存器子地址 (如果有) */
    if(xfer->SubaddressSize > 0) 
    {
        unsigned char *addr_buf = (unsigned char *)&(xfer->Subaddress);
        unsigned char i;
        
        /* 需要根据地址长度 (1/2/4字节) 进行发送，此处简化处理高位在前 */
        for(i = 0; i < xfer->SubaddressSize; i++)
        {
            base->I2SR &= ~(1 << 1); 
            
            /* 发送地址的高位字节 */
            if(xfer->SubaddressSize == 2)
                base->I2DR = addr_buf[i];
            else
                base->I2DR = addr_buf[xfer->SubaddressSize - 1 - i]; 

            while(!(base->I2SR & (1 << 1))); 
            
            ret = I2C_Check_And_Clear_Error(base, base->I2SR);
            if(ret) 
            {
                I2C_Master_Stop(base);
                return ret;
            }
        }

        /* 如果是读操作，需要发送 Repeated Start */
        if(xfer->Direction == I2C_Read) 
        {
            base->I2SR &= ~(1 << 1);
            I2C_Master_Repeat_Start(base, xfer->SlaveAddress, I2C_Read);
            
            while(!(base->I2SR & (1 << 1)));
            
            ret = I2C_Check_And_Clear_Error(base, base->I2SR);
            if(ret) 
            {
                I2C_Master_Stop(base);
                return ret;
            }
        }
    }

    /* 发送或读取数据 */
    if (xfer->Direction == I2C_Write && xfer->DataSize > 0)
        I2C_Master_Write(base, xfer->Data, xfer->DataSize);
    else if (xfer->Direction == I2C_Read && xfer->DataSize > 0)
        I2C_Master_Read(base, xfer->Data, xfer->DataSize);

    /* 发送 Stop 信号 */
    I2C_Master_Stop(base);
    return I2C_STATUS_OK;
}

