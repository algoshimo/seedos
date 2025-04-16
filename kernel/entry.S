.section .text
.global _entry
_entry:
    la sp, stack0         # 加载 start.c 文件定义的 stack0 地址
    li a0, 1024*4         # 每个栈大小为 4096 字节
    csrr a1, mhartid      # 获取 hartid
    addi a1, a1, 1        # hartid + 1
    mul a0, a0, a1        # 栈偏移 = 4096 * (hartid + 1)
    add sp, sp, a0        # sp = stack0 + 偏移

    call start            # 调用 start 函数 (定义在 start.c)
spin:
    j spin                # 进入死循环
