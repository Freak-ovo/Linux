
.global _start

_start:
    /* 设置处理器进入SVC模式 */
    MRS R0 , CPSR           /* 读取CPSR寄存器导 r0*/
    BIC R0 , R0 , #0x1f     /* 清除CPSR的bit 4：0 */
    ORR R0 , R0 , #0x13     /* 使用SVR模式 */
    MSR CPSR , R0           /* 将R0写入到CPSR */

    /* 设置SP指针 */
    LDR SP , = 0x80200000 /* 设置SP指针 */
    B main                /* 跳转到main函数 */
    
    
