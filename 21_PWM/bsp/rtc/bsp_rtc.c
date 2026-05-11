#include "bsp_rtc.h"
 





/* 初始化RTC */
void RTC_Init(void)
{
    rtc_datetime_t rtcdata;

    /* 初始化RTC寄存器 */
    SNVS->HPCOMR |= (1 << 31) | (1 << 8);


    /* 初始化RTC时间 */
    rtcdata.year = 2026;
    rtcdata.month = 5;
    rtcdata.day = 19;
    rtcdata.hour = 10;
    rtcdata.minute = 50;
    rtcdata.second = 0;


    /* 初始化RTC时间 */
    RTC_SetTime(&rtcdata);


    /* 使能RTC */
    RTC_Enable();
}

/* 使能RTC */
void RTC_Enable(void)
{
    SNVS->LPCR |= (1 << 0);
    /* 等待RTC使能完成 */
    while ((SNVS->LPCR & (1 << 0)) == 0);
        
}

/* 禁用RTC */
void RTC_Disable(void)
{
    SNVS->LPCR &= ~(1 << 0);
    /* 等待RTC禁用完成 */
    while ((SNVS->LPCR & (1 << 0)) != 0);
        
}
/* 判断是否为闰年 */
bool ETC_isleapyear(uint32_t year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/* 将时间结构体转换为秒数 */
uint64_t RTC_ConvertToSeconds(rtc_datetime_t *datetime)
{
    unsigned short i = 0;
    uint64_t seconds = 0;
    uint64_t days = 0;
    unsigned short monthdays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};
    for(i = 1970; i < datetime->year; i++)
    { days += DAY_IN_A_YEAR; /* 平年，每年365天 */
        if(ETC_isleapyear(i)) days += 1; /* 闰年多加一天 */ 
    }
    days += monthdays[datetime->month];
    if(ETC_isleapyear(datetime->year) && (datetime->month >= 3)) days += 1;
    days += datetime->day - 1;
    seconds = days * SECONDS_IN_DAY + datetime->hour * SECONDS_IN_HOUR + datetime->minute * SECONDS_IN_MINUTE + datetime->second;
    return seconds;
}

/* 设置RTC时间 */
void RTC_SetTime(rtc_datetime_t *datetime)
{
    unsigned int tmp = SNVS->LPCR;

    uint64_t seconds = 0;
    seconds = RTC_ConvertToSeconds(datetime);

    /* 设置RTC时间 */
    SNVS->LPSRTCMR = (unsigned int)(seconds >> 17); /* 高15位时间值写入LPSRTCMR寄存器 */
    SNVS->LPSRTCLR = (unsigned int)(seconds << 15);  /* 低17位时间值写入LPSRTCLR寄存器 */


    RTC_Disable();


    if (tmp & 0x01)   /* 判断RTC是否使能 */
    {
        RTC_Enable();
    }
}


/* 读取RTC时间秒数 */
uint64_t RTC_GetSeconds(void)
{
    uint64_t seconds = 0;
    seconds = ((uint64_t)(uint64_t)(SNVS->LPSRTCMR << 17) | SNVS->LPSRTCLR >> 15);    /* 从LPSRTCMR和LPSRTCLR寄存器读取时间值 */
    return seconds;
}

/* 获取时间 */
void RTC_GetTime(rtc_datetime_t *datetime)
{
    uint64_t seconds = 0;
    seconds = RTC_GetSeconds();
    datetime->year = 1970 + (uint32_t)(seconds / (SECONDS_IN_DAY * 365));
    datetime->month = (uint32_t)(seconds / (SECONDS_IN_DAY * 30) % 12) + 1;
    datetime->day = (uint32_t)(seconds / (SECONDS_IN_DAY) % 30) + 1;
    datetime->hour = (uint32_t)(seconds / (SECONDS_IN_MINUTE) % 60);
    datetime->minute = (uint32_t)(seconds / (SECONDS_IN_HOUR) % 60);
    datetime->second = (uint32_t)(seconds % 60);
}