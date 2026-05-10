.global _start          /* 声明 _start 为全局符号，供链接器识别，作为程序的绝对入口 */

/* 声明链接脚本中定义的 BSS 段边界符号（由链接脚本 imx6ull.lds 提供） */
.extern __bss_start     /* 引入 BSS 段的起始地址 */
.extern __bss_end       /* 引入 BSS 段的结束地址 */

.text                   /* 声明接下来的代码放入 .text 代码段 */
_start:
    /* 1. 设置处理器进入 SVC（超级用户）模式，并关闭中断 */
    mrs r0, cpsr        /* 将当前程序状态寄存器（CPSR）的值读取到 r0 中 */
    bic r0, r0, #0x1f   /* 清除 CPSR 的低5位（模式位），为切换模式做准备 */
    orr r0, r0, #0x13   /* 将 r0 的低5位设置为 0x13，即进入 SVC 管理模式 */
    orr r0, r0, #0xc0   /* 将 CPSR 的 I 位和 F 位置 1，禁止 IRQ（普通中断）和 FIQ（快速中断） */
    msr cpsr, r0        /* 将修改后的 r0 值写回 CPSR，配置正式生效 */

    /* 2. 清除 BSS 段（将未初始化的全局/静态变量所在内存清零） */
    ldr r0, =__bss_start       /* 将 BSS 段的起始地址加载到 r0 */
    ldr r1, =__bss_end         /* 将 BSS 段的结束地址加载到 r1 */
    mov r2, #0                 /* 准备用于清零的数值 0，放入 r2 */
bss_loop:
    str r2, [r0], #4           /* 将 0 存入 r0 指向的地址，然后 r0 自动加 4（字对齐递增） */
    cmp r0, r1                 /* 比较当前地址 r0 是否已经到达结束地址 r1 */
    blo bss_loop               /* 如果 r0 < r1 (Below/Lower)，说明还没清完，跳回 bss_loop 继续 */

    /* 3. 设置栈指针（SP），为 C 语言环境提供栈空间 */
    /* 栈通常设置在 SoC 内部 RAM 或 DDR 的高地址处，根据你的具体芯片内存映射调整 */
    ldr sp, =0x80200000        /* 将栈指针指向 0x80200000（i.MX6ULL 常用的 DDR 起始偏移地址） */

    /* 4. 跳转到 C 语言 main 函数，正式进入应用逻辑 */
    b main                     /* 无条件跳转到 main 函数执行（注意：此跳转不会自动保存返回地址） */