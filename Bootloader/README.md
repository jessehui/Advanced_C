
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

*跳转到main
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

*Android专有的驱动程序

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
