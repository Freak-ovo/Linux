#include "bsp_key.h"
#include "bsp_delay.h"

#include "bsp_gpio.h"
/*
 *  按键KEY0连接到了UART1_CTS引脚上。默认情况UART1_CTS引脚为高电平，按键按下时为低电平
 * 1. 初始化UART1_CTS这个IO 复用为GPIO_IO18
 * 2. 设置UART1_CTS这个IO的电器属性
 * 3. 配置GPIO1_IO18为输出模式
 * 4. 读取按键状态 当按键按下时，GPIO1_IO18引脚接收到的电平为低电平
 *
 * *
*/
/* 按键初始化 */
void Key_Init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);  /* 复用IO */

    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0X080);  /* 设置IO电器属性 */

    /* GPIO初始化 */
    //GPIO1->GDIR  &= ~(1 << 18);    /* 将bit18设置为0，设置为输入 */
    gpio_pin_config_t gpio_pin_config;
    gpio_pin_config.direction = kGPIO_DigitalInput;
    
    gpio_init(GPIO1, 18, &gpio_pin_config);

}

/* 按键读取函数 */
int Key_Read(void)
{
    int ret = 0;
    //ret = ((GPIO1->DR) >> 18) & (0X1);  /* 读取bit18的值 */
    ret = gpio_pinread(GPIO1, 18);
    return ret;
}
/* 按键消抖 */
int Key_GetValue(void)
{
    int ret = KEY_NONE;
    static unsigned char release = 1;   /* 按键是否释放 0：未释放 1：已释放 */
    
    //if ((release == 1) && (Key_Read() == 0))    /* 按键按下时 */
    if ((release == 1) && (gpio_pinread(GPIO1, 18) == 0))    /* 按键按下时 */
    {
        Delay(10);
        release = 0;
        //if (Key_Read() == 0)    /* 如果延时10ms后，按键还是按下状态，表示按键按下状态 */
        if (gpio_pinread(GPIO1, 18) == 0)    /* 如果延时10ms后，按键还是按下状态，表示按键按下状态 */
        {
            ret = KEY0_VALUE;
        }
    }
    //else if ((release == 0) && (Key_Read() == 1))    /* 按键释放时 */
    else if((release == 0) && (gpio_pinread(GPIO1, 18) == 1))
    {
        Delay(10);
        release = 1;
        //if (Key_Read() == 1)    /* 如果延时10ms后，按键还是释放状态，表示按键释放状态 */
        if (gpio_pinread(GPIO1, 18) == 1)    /* 如果延时10ms后，按键还是释放状态，表示按键释放状态 */

        {
            ret = KEY_NONE;
        }
    }


    return ret;
}
