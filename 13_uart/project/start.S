.global _start          /* 声明 _start 为全局符号，供链接器识别，作为程序的绝对入口 */


_start:
    ldr pc, =Reset_Handler      /* 复位中断（Reset）：芯片上电后执行的第一条指令 */
    ldr pc, =Undefine_Handler   /* 未定义指令中断（Undefined Instruction） */
    ldr pc, =SVC_Handler        /* 超级用户调用中断（Supervisor Call, SVC） */
    ldr pc, =PreAbort_Handler   /* 预取指令异常中断（Prefetch Abort） */
    ldr pc, =DataAbort_Handler  /* 数据异常中断（Data Abort） */
    ldr pc, =NotUsed_Handler    /* 保留中断（未使用） */
    ldr pc, =IRQ_Handler        /* 普通中断请求（IRQ） */
    ldr pc, =FIQ_Handler        /* 快速中断请求（FIQ） */
    
/* 声明链接脚本中定义的 BSS 段边界符号（由链接脚本 imx6ull.lds 提供） */
.extern __bss_start     /* 引入 BSS 段的起始地址 */
.extern __bss_end       /* 引入 BSS 段的结束地址 */

.text                   /* 声明接下来的代码放入 .text 代码段 */
/* 复位中断处理函数 */
Reset_Handler:
    cpsid i                     /* 关闭全局 IRQ 中断，防止在初始化过程中被意外打断 */

    /* 关闭 I-Cache、D-Cache、MMU 以及分支预测等，保证初始化的稳定性 */
    mrc p15, 0, r0, c1, c0, 0   /* 读取协处理器 CP15 的系统控制寄存器（SCTLR）的值到 r0 */
    bic r0, r0, #(1 << 12)      /* 清除 bit12：关闭指令缓存（I-Cache） */
    bic r0, r0, #(1 << 11)      /* 清除 bit11：关闭分支预测（Branch Prediction） */
    bic r0, r0, #(1 << 2)       /* 清除 bit2：关闭数据缓存（D-Cache） */
    bic r0, r0, #(1 << 1)       /* 清除 bit1：关闭地址对齐检查（Alignment Check） */
    bic r0, r0, #(1 << 0)       /* 清除 bit0：关闭内存管理单元（MMU） */
#if 0
    /* 设置中断向量表的偏移地址（VBAR） */
    ldr r0, =0x87800000         /* 设置向量表基地址为 DDR 中的 0x87800000 */
    dsb                         /* 数据同步屏障，确保前面的内存访问操作全部完成 */
    isb                         /* 指令同步屏障，清空流水线，确保后续指令从新环境取指 */
    mcr p15, 0, r0, c12, c0, 0  /* 将基地址写入协处理器 CP15 的 VBAR 寄存器 */
    dsb                         /* 数据同步屏障 */
    isb                         /* 指令同步屏障 */
#endif
    mcr p15, 0, r0, c1, c0, 0   /* 将 r0 中配置好的值（已关闭 Cache/MMU 等）写回 SCTLR 寄存器，配置正式生效 */

    /* 清除 BSS 段（将未初始化的全局变量和静态变量所在内存清零） */
    ldr r0, =__bss_start        /* 将 BSS 段的起始地址加载到 r0 */
    ldr r1, =__bss_end          /* 将 BSS 段的结束地址加载到 r1 */
    mov r2, #0                  /* 准备用于清零的数值 0，放入 r2 */
bss_loop:
    str r2, [r0], #4            /* 将 0 存入 r0 指向的地址，然后 r0 自动加 4（字对齐递增） */
    cmp r0, r1                  /* 比较当前地址 r0 是否已经到达结束地址 r1 */
    blo bss_loop                /* 如果 r0 < r1 (Below/Lower)，说明还没清完，跳回 bss_loop 继续 */

    /* 1. 设置处理器进入 IRQ 模式，并配置栈指针 */
    mrs r0, cpsr                /* 将当前程序状态寄存器（CPSR）的值读取到 r0 中 */
    bic r0, r0, #0x1f           /* 清除 CPSR 的低5位（模式位），为切换模式做准备 */
    orr r0, r0, #0x12           /* 将 r0 的低5位设置为 0x12，即进入 IRQ 中断模式 */
    msr cpsr, r0                /* 将修改后的 r0 值写回 CPSR，模式切换正式生效 */
    ldr sp, =0x80600000         /* 设置 IRQ 模式下的栈指针（SP），指向 DDR 地址 0x80600000 */
    
    /* 2. 设置处理器进入 SYS 模式，并配置栈指针 */
    mrs r0, cpsr                /* 将当前程序状态寄存器（CPSR）的值读取到 r0 中 */
    bic r0, r0, #0x1f           /* 清除 CPSR 的低5位（模式位），为切换模式做准备 */
    orr r0, r0, #0x1f           /* 将 r0 的低5位设置为 0x1F，即进入 SYS 系统模式（与USR模式寄存器共用） */
    msr cpsr, r0                /* 将修改后的 r0 值写回 CPSR，模式切换正式生效 */
    ldr sp, =0x80400000         /* 设置 SYS 模式下的栈指针（SP），指向 DDR 地址 0x80400000 */

    /* 3. 设置处理器进入 SVC 模式，并配置栈指针 */
    mrs r0, cpsr                /* 将当前程序状态寄存器（CPSR）的值读取到 r0 中 */
    bic r0, r0, #0x1f           /* 清除 CPSR 的低5位（模式位），为切换模式做准备 */
    orr r0, r0, #0x13           /* 将 r0 的低5位设置为 0x13，即进入 SVC 管理模式 */
    msr cpsr, r0                /* 将修改后的 r0 值写回 CPSR，模式切换正式生效 */
    ldr sp, =0x80200000         /* 设置 SVC 模式下的栈指针（SP），指向 DDR 地址 0x80200000 */

    cpsie i                     /* 开启全局 IRQ 中断 */

    /* 跳转到 C 语言 main 函数，正式进入应用逻辑 */
    b main                      /* 无条件跳转到 C 语言的 main 函数执行 */

/* 未定义指令中断服务函数 */
Undefine_Handler:
    ldr r0, = Undefine_Handler
    bx r0

/* SVC（超级用户调用）中断服务函数 */
SVC_Handler:
    ldr r0, = SVC_Handler
    bx r0

/* 预取指令异常中断服务函数 */
PreAbort_Handler:
    ldr r0, = PreAbort_Handler
    bx r0

/* 数据异常中断服务函数 */
DataAbort_Handler:  
    ldr r0, = DataAbort_Handler
    bx r0

/* 保留中断服务函数（未使用） */
NotUsed_Handler:
    ldr r0, = NotUsed_Handler
    bx r0

/* IRQ（普通中断）中断服务函数 */
IRQ_Handler:
    push {lr}                   /* 保存 lr 寄存器（返回地址） */
    push {r0-r3, r12}           /* 保存 r0-r3, r12 寄存器（遵循 AAPCS 调用约定，保护现场） */

    mrs r0, spsr                /* 读取备份程序状态寄存器（SPSR）的值到 r0 */
    push {r0}                   /* 保存 SPSR 的值 */

    mrc p15, 4, r1, c15, c0, 0  /* 读取 GIC（通用中断控制器）的 CPU 接口基地址到 r1 */

    add r1, r1, #0x2000         /* 计算获取 GIC 分发器的基地址 */
    ldr r0, [r1, #0XC]          /* 读取分发器的 IAR 寄存器（中断应答寄存器），获取当前触发的中断号 */

    push {r0, r1}               /* 保存中断号和 GIC 分发器基地址 */
    
    cps #0x13                   /* 切换到 SVC 模式（准备调用 C 语言中断处理函数） */

    push {lr}                   /* 保存 SVC 模式下的 lr */

    ldr r2, = system_irqhandler /* 加载 C 语言编写的总中断处理函数地址到 r2 */
    blx r2                      /* 带状态切换地跳转到 C 语言函数执行 */

    pop {lr}                    /* 恢复 SVC 模式下的 lr */
    cps #0x12                   /* 切换回 IRQ 模式 */
    pop {r0, r1}                /* 恢复中断号到 r0，GIC 分发器基地址到 r1 */
    str r0, [r1, #0x10]         /* 将中断号写入 GIC 分发器的 EOIR 寄存器（中断结束寄存器），通知 GIC 中断处理完毕 */

    pop {r0}                    /* 恢复之前保存的 SPSR 值到 r0 */
    msr spsr_cxsf, r0           /* 将 r0 的值写回 SPSR，恢复中断前的程序状态 */

    pop {r0}                    /* 恢复 r0（占位弹出，平衡堆栈） */
    pop {lr}                    /* 恢复 lr（返回地址） */
    subs pc, lr, #4             /* 带状态切换地返回（减去4是因为 ARM 流水线机制），跳回被中断的指令处继续执行 */

    
/* FIQ（快速中断）中断服务函数 */
FIQ_Handler:
    ldr r0, = FIQ_Handler
    bx r0
