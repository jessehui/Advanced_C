
# Bootloader Development Note

###1
boot目的:
跳到C语言中(让sp堆栈指针指向可读可写的设备中)
ATPCS标准:ATPCS即ARM-THUMB procedure call standard（ARM-Thumb过程调用标准）的简称。
PCS强制实现如下约定：调用函数如何传递参数（即压栈方法，以何种方式存放参数），被调用函数如何获取参数，以何种方式传递函数返回值。
```Assembly
MOV sp, #200000
BL MAIN
```

loader目的: 执行应用逻辑,点灯,uart, load Linux kernel


###2
