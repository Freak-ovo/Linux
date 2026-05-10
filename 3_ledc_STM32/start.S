.global _start

/* 声明链接脚本中定义的 BSS 边界符号（由链接脚本提供） */
.extern __bss_start
.extern __bss_end

.text
_start:
    /* 1. 设置处理器进入 SVC 模式，关闭 IRQ/FIQ */
    mrs r0, cpsr
    bic r0, r0, #0x1f          /* 清除模式位 */
    orr r0, r0, #0x13          /* 设置为 SVC 模式 */
    orr r0, r0, #0xc0          /* 禁止 IRQ 和 FIQ */
    msr cpsr, r0

    /* 2. 清除 BSS 段 */
    ldr r0, =__bss_start       /* BSS 起始地址 */
    ldr r1, =__bss_end         /* BSS 结束地址（紧接最后一个字节的下一个地址） */
    mov r2, #0
bss_loop:
    str r2, [r0], #4           /* 写入 0 并递增地址，比 stmia 更直观 */
    cmp r0, r1
    blo bss_loop               /* 若 r0 < r1，继续清除 */

    /* 3. 设置栈指针 */
    ldr sp, =0x80200000        /* 根据你的 SoC 内存地址调整（i.MX6ULL 常用 0x80200000） */

    /* 4. 跳转到 C 语言 main 函数 */
    b main