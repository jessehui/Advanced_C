
# Bootloader Development Note

###1
boot目的:

* 关闭看门狗 关闭中断,MMU(有操作系统情况下使用),cache(打开影响数据完整性 建议关闭)
MMU:内存管理单元（英语：memory management unit，缩写为MMU），有时称作分页内存管理单元（英语：paged memory management unit，缩写为PMMU）。它是一种负责处理中央处理器（CPU）的内存访问请求的计算机硬件。它的功能包括虚拟地址到物理地址的转换（即虚拟内存管理）、内存保护、中央处理器高速缓存的控制，在较为简单的计算机体系结构中，负责总线的仲裁以及存储体切换（bank switching，尤其是在8位的系统上）。

* 配置系统工作时钟

* 配置SDRAM控制器(行地址数,列地址数,多少块,周期性的充电(需要分频器 需要配置系统时钟))
同步动态随机存取內存（synchronous dynamic random-access memory，简称SDRAM）是有一个同步接口的动态随机存取內存（DRAM）。通常DRAM是有一个异步接口的，这样它可以随时响应控制输入的变化。而SDRAM有一个同步接口，在响应控制输入前会等待一个时钟信号，这样就能和计算机的系统总线同步。

* 跳到C语言中(让sp堆栈指针指向可读可写的设备中) 满足递减栈的规则(sp指针往下走 尽量放高段地址)
ATPCS标准:ATPCS即ARM-THUMB procedure call standard（ARM-Thumb过程调用标准）的简称。
PCS强制实现如下约定：调用函数如何传递参数（即压栈方法，以何种方式存放参数），被调用函数如何获取参数，以何种方式传递函数返回值。
用哪些模式(SVC,IRQ,USR等)就初始化哪些模式下的;每个模式的值不能覆盖其他模式

* 代码搬移
执行速度问题,比如把存储器(nand-flash)搬移到内存上. 需要对对应存储器的控制器进行初始化

* 跳转到main
```Assembly
MOV sp, #200000
BL MAIN
```
虽然Android基于Linux内核，但是它与Linux之间还是有很大的差别，比如Android在Linux内核的基础上添加了自己所特有的驱动程序.

* 它没有本地窗口系统
　　什么是本地窗口系统呢?本地窗口系统是指GNU/Linux上的X窗口系统，或者Mac OX X的Quartz等。不同的操作系统的窗口系统可能不一样，Android并没有使用(也不需要使用)Linux的X窗口系统，这是Android不是Linux的一个基本原因。

* 它没有glibc支持
　　由于Android最初用于一些便携的移动设备上，所以，可能出于效率等方面的考虑，Android并没有采用glibc作为C库，而是Google自己开发了一套Bionic Libc来代替glibc。

* 它并不包括一整套标准的Linux使用程序
　　Android并没有完全照搬Liunx系统的内核，除了修正部分Liunx的Bug之外，还增加了不少内容，比如：它基于ARM构架增加的Gold-Fish平台，以及yaffs2 FLASH文件系统等。

* Android专有的驱动程序

glibc是gnu发布的libc库，也即c运行库。glibc是linux 系统中最底层的api（应用程序开发接口），几乎其它任何的运行库都会倚赖于glibc。glibc除了封装linux操作系统所提供的系统服务外，它本身也提供了许多其它一些必要功能服务的实现，主要的如下：
（1）string，字符串处理
（2）signal，信号处理
（3）dlfcn，管理共享库的动态加载
（4）direct，文件目录操作
（5）elf，共享库的动态加载器，也即interpreter
（6）iconv，不同字符集的编码转换
（7）inet，socket接口的实现
（8）intl，国际化，也即gettext的实现
（9）io
（10）linuxthreads
（11）locale，本地化
（12）login，虚拟终端设备的管理，及系统的安全访问
（13）malloc，动态内存的分配与管理
（14）nis
（15）stdlib，其它基本功能

<br/>
<br/>
loader目的: 执行应用逻辑,点灯,uart, load Linux kernel


###2
U-Boot: 全称 Universal Boot Loader，是遵循GPL条款的开放源码项目。从FADSROM、8xxROM、PPCBOOT逐步发展演化而来。其源码目录、编译形式与Linux内核很相似，事实上，不少U-Boot源码就是根据相应的Linux内核源程序进行简化而形成的，尤其是一些设备的驱动程序，这从U-Boot源码的注释中能体现这一点。

 Flash编程原理都是只能将1写为0，而不能将0写成1.所以在Flash编程之前，必须将对应的块擦除，而擦除的过程就是将所有位都写为1的过程，块内的所有字节变为0xFF.因此可以说，编程是将相应位写0的过程，而擦除是将相应位写1的过程，两者的执行过程完全相反.

 最先出现的Nor flash，采用并口输入输出数据，速度快，CPU可以直接从Nor Flash中取指令执行专门术语叫XIP (eXecute In Place），但是价格昂贵，同时，由于要占用地址线，容量不能很大。基于以上的缺点，三星公司开发了Nand Flash结构，这种结构采用串行数据存储，而且使用块存储，故而容量可以很高。但是它的缺点是程序不能直接在Nand Flash中运行，只能拷贝到NorFlash或者是RAM中运行，这样程序启动的过程中就存在一个程序拷贝的过程，这个过程主要在Nor Flash中运行。

所以，一般NorFlash用作存放BootLoader（这部分内容也类似于PC中的BIOS和MBR），主要是在单片机上电后，执行将Nand Flash中的程序拷贝到RAM中，然后将CPU控制权交给RAM中的程序（用户自己编写的函数或者是系统内核）。所以，一般的嵌入式系统中都带有容量较小的Nor Flash和大容量的Nand Flash。Nor Flash主要用来存放引导程序，而Nand Flash存放用户真正的程序。如果使用操作系统，一般内核比较大，系统启动过程比较慢，大约5-10秒不等，这个过程中主要执行开机自检和硬件初始化以及程序拷贝。
 由于NorFlash地址线和数据线分开，所以NorFlash芯片可以像SDRAM一样连在数据线上.NOR芯片的使用类似于通常内存芯片，传输效率高，可执行程序可以在芯片内执行(XI P, eXecute In Place)，这样应用程序可以直接在flash闪存内运行，不必再把代码读到系统RAM中.由于NorFlash的这个特点，嵌入式系统中经常将NOR芯片做启动芯片使用.NandFlash共用地址和数据总线，需要额外联结一些控制的输入输出，所以直接将NAND芯片做启动芯片比较难.

S5PV210启动时从NandFlash的0地址拷贝时候会拷贝前16k，但是会读取前16byte进行与和校验值做比较。

###3
Makefile:
makefile关系到了整个工程的编译规则。一个工程中的源文件不计数，其按类型、功能、模块分别放在若干个目录中，makefile定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作，因为makefile就像一个Shell脚本一样，其中也可以执行操作系统的命令。 

规则:
变量集合
target ... : prerequisites ...
   command

通用Makefile,支持SD卡启动和在uboot下直接在ram运行:
(1) 程序运行时基地址不同
(2) SD卡需要加一个头信息 校验; ram不需要加头

 编译过程:
 很多.c文件 ----> 很多.o文件 -----> build ------> build.bin
Makefile倒着写

工具objcopy可以用于从一个文件拷贝目标代码到另一个文件，并在这一过程中执行各种转换。通过使用objcopy，我们可以在不同的目标代码格式之间进行自动的转换并操纵这一过程中的内容。

objcopy - copy and translate object files
 [-O bfdname|--output-target=bfdname]
 It can write the destination object file in a format different
from that of the source object file.  The exact behavior of objcopy is
controlled by command-line options.  Note that objcopy should be able
to copy a fully linked file between any two formats.

