#ifndef __CC_H__
#define __CC_H__

/* CMSIS style access modifiers */
#define __I  volatile const   /* 只读，通常需要 const */
#define __O  volatile         /* 只写 */
#define __IO volatile         /* 读写 */


#define ON      1
#define OFF     0

/* 包含标准类型定义，避免手动 typedef 冲突 */
#include <stdint.h>

/* 如果某些环境没有 stdint.h，可以手动定义（但推荐使用标准头文件） */
#ifndef HAVE_STDINT_H
typedef unsigned char        uint8_t;
typedef unsigned short       uint16_t;
typedef unsigned int         uint32_t;
typedef unsigned long long   uint64_t;
#endif

/* 有符号版本，短名称 */
typedef signed char          s8;
typedef signed short         s16;
typedef signed int           s32;

/* 无符号短名称 */
typedef unsigned char        u8;
typedef unsigned short       u16;
typedef unsigned int         u32;
typedef unsigned long long   u64;

#endif /* __CC_H__ */