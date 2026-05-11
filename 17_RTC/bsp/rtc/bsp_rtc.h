#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

#include "imx6ull.h"


/* 跟时间有关系的宏定义 */
#define SECONDS_IN_DAY             (24 * 60 * 60) /* 一天的秒数 */
#define SECONDS_IN_HOUR            (60 * 60) /* 一个小时的秒数 */
#define SECONDS_IN_MINUTE          (60) /* 一分钟的秒数 */
#define DAY_IN_A_YEAR               (365) /* 一年的天数 */
#define YEAR_RANGE_START            (1970) /* 年范围的开始值 */
#define YEAR_RANGE_END              (2099) /* 年范围的结束值 */


/* 跟时间有关系的结构体 */
typedef struct {
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} rtc_datetime_t;



void RTC_Init(void);
void RTC_Enable(void);
void RTC_Disable(void);

uint64_t RTC_ConvertToSeconds(rtc_datetime_t *datetime);
void RTC_SetTime(rtc_datetime_t *datetime);
uint64_t RTC_GetSeconds(void);
void RTC_GetTime(rtc_datetime_t *datetime);

#endif /* __BSP_RTC_H__ */