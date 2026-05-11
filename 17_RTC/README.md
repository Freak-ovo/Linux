# RTC(Real Time Clock)
## 一、RTC介绍
> 1. IMX6ULL 内部自带到了一个 RTC 外设 ，确切的说是 SRTC。 IMX6U 和 IMX6ULL 的 RTC 内容在 SNVS 章节，6U 的 RTC 分为 LP 和 HP。LP叫做 SRTC，HP 是 RTC。但是 HP 的 RTC 掉电以后数据就丢失了，即使用了纽扣电池也没用，所以必须要用LP，也就是 SRTC。
> 2. SNVS 章节 有些是跟加密有关系的，需要和 NXP 签订 NDA 协议才能拿到。
> 3. RTC 分为 SNVS_LP 和 SNVS_HP。
> 4. 如果做产品，建议使用外置 RTC 芯片， PCF8563。
> 5. RTC 类似定时器，外接 32.768kHz 晶振，然后就开始计时， RTC 使用俩个寄存器来保存计数值。
> 5. RTC 使用很简单，打开 RTC，然后 RTC 就开始工作，我们要做的就是不断地读取 RTC 寄存器，获取时间值，或者向 RTC 寄存器写入时间值，也就是调整时间。
> 6. SNVS_LPCR 寄存器，bit[0] 置 [1]，开启 SRTC 功能。
> 7. SNVS_LPSRTCMR 是高 15 位 RTC 计数寄存器。每1s 数字加 1。
> 8. SNVS_LPSRTCLR 是低 15 位 RTC 计数寄存器。与 SNVS_LPSRTCMR 合起来，组成 30 位的 RTC 计数寄存器。 每隔 1s，SNVS_LPSRTCLR 会自动加 1。
> 9. IMX6ULL 的 RTC 模式从 1970年1月1日 0时0分0秒 开始计时。
## 二、问题总结
### 1. 乱码问题
> 1. 当我们按照6U的参考手册编写代码，读取 SRTC 的 LPSRTCMR 和 LPSRTCLR 获取时间值的时候，发现按照手册的说法，发现时间值是乱码的。
> 2. 手册上写的：LPSRTCMR 是 SRTC 的高 15bit。LPSRTCLR 是低 32bit。RTC 计数器是47bit
### 2. 解决方法
> 1. LPSRTCMR 作为 SRTC 的高 15位，但是LPSRTCLR寄存器bit[31:15] 作为 SRTC的低17位，相当于SRTC的计数器是个32位的。不是47位。
```c
seconds = ((uint64_t)(uint64_t)(SNVS->LPSRTCMR << 17) |
          SNVS->LPSRTCLR >> 15);    /* 从 LPSRTCMR 和 LPSRTCLR 寄存器读取时间值 */
