OUTPUT_FORMAT("elf32-littlearm","elf32-littlearm","elf32-littlearm)    /*指定输出可执行文件是elf格式,32位ARM指令,小端*/
OUTPUT_ARCH(arm)                    /*#指定输出可执行文件的平台为ARM*/
ENTRY(_start)                       #指定输出可执行文件的起始代码段为_start.
SECTIONS
{
    . = 0x00000000;                 #定位当前地址为0地址
    . = ALIGN(4);                   #代码以4字节对齐
    .text :                         #指定代码段：必须将start.o文件放在代码段的开始位置，其它文件可任意放
    {
        start.o (.text) #代码段第一部分，指明start.s是入口程序，被放到代码段开头
        *(.text)                    #其它代码部分.其中的*表示其它任意文件，即所有其它文件的代码段
    }

    . = ALIGN(4);
    .rodata : { *(.rodata) }        #指定只读数据段，RO段

    . = ALIGN(4);
    .data : { *(.data) }            #指定读/写数据段，RW段

    . = ALIGN(4);
    .got : { *(.got) }              #指定got段, got段式是uboot自定义的一个段, 非标准段
    __u_boot_cmd_start = .          #把__u_boot_cmd_start赋值为当前位置, 即起始位置
    .u_boot_cmd : { *(.u_boot_cmd) }#指定u_boot_cmd段, uboot把所有的uboot命令放在该段.
    __u_boot_cmd_end = .            #把__u_boot_cmd_end赋值为当前位置,即结束位置

    . = ALIGN(4);
    __bss_start = .                 #把__bss_start赋值为当前位置,即bss段的开始位置
    .bss : { *(.bss) }              #指定bss段
    _end = .                        #把_end赋值为当前位置,即bss段的结束位置
}