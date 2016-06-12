#uCOS Study Note

1 操作系统分类

（1）分时操作系统
使一台计算机采用时间片轮换的方式同时为几个几十个甚至几百个用户服务的一种操作系统。例如windows
（2）实时操作系统
保证在一定时间限度内完成特定功能的操作系统。分为硬实时和软实时 前者要求在规定的时间内必须完成操作 这是在操作系统设计时就要保证得，软实时则只要保证任务的优先级，尽可能快的完成即可。UCOS介于硬实时和软实时操作系统之间

2

(1)前后台系统：
```C
int main()
{
     initLED();
     initUART();
     initKEY();
     while(1)
     {
          ...
          ...
     }
}
```
前台代表中断，后台代表while
对任务级任务的响应时间不确定
及时性稍差.

（2）多任务系统
在一个程序中有两个或多个类似main函数的块  对这些块进行快速调度 就好像他们在同时执行
实现调度的称为内核
任务的响应时间确定 比较及时

3 内核

在多任务系统中，内核负责管理各个任务，或者说内核为各个任务分配运行时间。另一个作用是负责任务间的通信。
内核调度方法：时间片轮转调度法（linux,windows ）和优先级调度法(UCOS-II)
优先级调度法：可剥夺型内核 不可剥夺型内核
不可剥夺型内核：T2执行时，即使更高优先级的T1要执行，也要等待T2执行完后
可重入函数 和 不可重入函数
```C
//不可重入函数
int temp;
void swap(int *x, int *y)
{
     temp = *x;
     *x = *y;
     *y = temp;
}
```
不可重入函数一旦被打断，再次进入该函数会出现混乱。其实就是因为使用了全局变量temp

```C
//可重入函数
void strcpy(char *dest, char *src, int len)
{
     while(len--)
     {
          *dest++ = *src++;
     }
}
```
造成的函数的不可重入其实就是因为使用了全局变量。因为局部变量保存在堆栈中，每个任务都有自己的堆栈。
一般来说 在可剥夺型内核中，要使用可重入函数。如果使用不可剥夺型内核可以使用不可重入函数。

4

UCOS采用可剥夺型内核 每一个任务都有优先级 使用一个八位整型数据来表示
。支持动态优先级
优先级继承：UCOS不支持。当某个任务使用全局变量时，内核能够自动提升该任务的优先级，当全局变量释放后，能够恢复该任务的优先级。这个过程称为优先级继承。
优先级分配规则：实时性要求高的（例如UART），运行速度快的，在逻辑任务之前的（例如键盘初始化）应分配较高优先级。

5

代码临界段：指处理时不可分割的代码 一旦这段代码开始执行，就不能被中断打断。可以采用关闭中断的方式 执行完临界段代码后再开中断
资源：任何为任务所占用的实体
共享资源：被一个以上任务使用的资源。对共享资源的访问 两个或以上任务必须满足互斥（独占）的条件
满足互斥的方法：1.关中断 开中断      2.测试并置位指令     3.任务禁止切换，使用，任务允许切换      4.使用信号量（类似2）获取 使用 释放 

6 死锁

也称抱死。两个或以上任务无限期的互相等待对方控制的资源

7 同步和通信

同步：两个任务之间满足某种关系 但没有数据的交互
通信：两个任务之间有数据的交互

8 中断

中断延迟：CPU从中断产生到中断执行的时间，这段时间用于保护现场
ISR: interrupt service routine 中断服务程序
```C
OS_IntEnter( );//进入中断函数
OS_IntExit( );//退出中断函数
```
原则：中断时间越短越好 过长会影响任务的实时性
时间节拍：硬件定时器。定时时间越短，系统实时性越好，但是无用功更多。

9 uCOS初始化

（1）初始化操作系统 OS_Init( ); 即初始化系统提供的各种服务如时间节拍，消息邮箱服务等
（2）创建任务
（3）执行操作系统 OS_Start( );

10 uCOS编程规约

对于操作系统提供的服务都已OS开头 比如 OS_TaskCreate( );
缩写、略语和助记符：Addr(address), Blk(block), Chk(check)

11

通信录功能所需硬件资源：
LED，蜂鸣器，输入按键，TFLCD屏幕，uart串口可以用于打印信息

12

OSTaskCreateExt 任务创建
INT16U opt参数：取值范围有3种
OS_TASK_OPT_STK_CHK 使用任务堆栈检查功能
OS_TASK_OPT_STK_CLR 使用任务堆栈清零功能
OS_TASK_OPT_SAVE_FP 如果硬件支持浮点数运算 可以用浮点数来保存一些信息

13

为什么每个任务都需要堆栈？保存任务的地址 避免地址冲突 要使用不同的空间作为堆栈
堆栈的数据类型OS_STK 在不同的硬件平台上的大小不同。32位CPU为占32位的无符号整型数unsigned int
合理设置任务堆栈大小：过大浪费内存空间（静态数组 不会释放）
会占用堆栈的资源：函数调用 局部变量 函数入口参数 函数返回值
原则：先设置一个较大的堆栈然后逐步减小 直到合适为止
可以使用uCOS提供的堆栈检查功能
INT8U OSTaskStkChk(    INT8U prio,
            OS_STK_DATA *p_stk_data
)
必须长时间运行才会比较准确
STM32任务堆栈为向上递增的堆栈 栈顶为最后一个元素
（堆栈分为向上递增和向下递减 递增栈顶为最后一个元素 递减栈顶为第一个元素 向上还是向下和CPU平台有关系）

14

任务一般都是自己删除自己 不要删除别的任务 因为别的任务可能正在运行 删除会造成错误
任务的挂起OSTaskSuspend()和任务的删除OSTaskDel()是不同的。挂起的任务可以恢复运行OSTaskResume()，任务的删除就是该任务不存在了。挂起所占用的内存不会释放，删除后，所占用的内存会释放掉。不允许挂起空闲任务。

15

OSTaskNameSet()和OSTaskNameGet()这两个函数不能在中断服务程序中调用

16

在操作系统运行之前，利用OSTimeSet( )设置初始时间为0

17任务的同步和通信

同步：两个任务有序进行 e.g. 按下一个按键 点亮一个LED
通信：不同任务之间数据的交互
uCOS使用事件来管理任务间的同步和通信。把信号量，事件标志组，消息邮箱，消息队列统称为事件。

```C
//Event 类型定义
#if OS_LOWEST_PRIO <= 63u
typedef  INT8U    OS_PRIO;
#else
typedef  INT16U   OS_PRIO;
#endif

#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
typedef struct os_event {
    INT8U    OSEventType;                    /* Type of event control block (see OS_EVENT_TYPE_xxxx)    */
    void    *OSEventPtr;                     /* Pointer to message or queue structure                   */
    INT16U   OSEventCnt;                     /* Semaphore(信号) Count (not used if other EVENT type)          */
    OS_PRIO  OSEventGrp;                     /* Group corresponding to tasks waiting for event to occur */
    OS_PRIO  OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur                */

#if OS_EVENT_NAME_EN > 0u
    INT8U   *OSEventName;
#endif
} OS_EVENT;
#endif
```

18

信号量（Sem）:semaphore 也有旗语的意思
配置：关于此操作都在os_cfg.h中
二值信号量：0，1 通常使用在对共享资源的保护
计数型信号量：初始值是大于1
信号量使用方法：
```C
OS_EVENT  *OSSem_Creat(INT16U cnt)//建立 返回一个事件
void  OSSemPend (OS_EVENT  *pevent,
                 INT32U     timeout,
                 INT8U     *perr)//等待 不允许在中断中等待
INT8U  OSSemPost (OS_EVENT *pevent)//发送 返回一个状态信息
//OS_ERR_NONE         The call was successful and the semaphore was signaled.
//OS_ERR_SEM_OVF ...
```
句柄与普通指针的区别在于，指针包含的是引用对象的内存地址，而句柄则是由系统所管理的引用标识，该标识可以被系统重新定位到一个内存地址上。这种间接访问对象的模式增强了系统对引用对象的控制。通俗的说就是我们调用句柄就是调用句柄所提供的服务，即句柄已经把它能做的操作都设定好了，我们只能在句柄所提供的操作范围内进行操作，但是普通指针的操作却多种多样，不受限制。

OSSemPend 是一个阻塞式等待过程,所以不能用于中断服务函数中.

OSSemAccept
```C
INT16U  OSSemAccept (OS_EVENT *pevent) //查询资源是否可用
```
非阻塞式等待过程,可以用于中断服务函数

```C
OS_EVENT  *OSSemDel (OS_EVENT  *pevent,
                     INT8U      opt,
                     INT8U     *perr)//删除任务
```
一般信号量不被任何任务等待的时候才可以删除. 返回值为空指针 如果删除成功的话

```C
void  OSSemSet (OS_EVENT  *pevent,
                INT16U     cnt,
                INT8U     *perr)//设置信号量资源数量,cnt 置为0时 表示重置该信号量
```

19

互斥性信号量(Mutex) 类似信号量中的二值信号量. 经常用来对共享资源进行保护. 对优先级反转进行解决
需要建立 等待 发送过程.类似信号量(Sem)
```C
OS_EVENT  *OSMutexCreate (INT8U   prio,//要提高到哪个优先级 必须是高于可能使用该信号量的所有任务的优先级
                          INT8U  *perr)

void  OSMutexPend (OS_EVENT  *pevent,
                   INT32U     timeout,
                   INT8U     *perr)//等待

INT8U  OSMutexPost (OS_EVENT *pevent)//发送
```
例如串口作为共享资源,如果在led beep key任务中都需要使用串口打印, 就需要对串口资源进行保护

20

事件标志组(Flag) uCOS 默认用一个16位的数表示
```C
OS_FLAG_GRP  *OSFlagCreate (OS_FLAGS  flags,
                            INT8U    *perr)

OS_FLAGS  OSFlagPend (OS_FLAG_GRP  *pgrp,
                      OS_FLAGS      flags,
                      INT8U         wait_type,//全为0 全为1 任何一位被置0或置1

 //                                 Example, to wait for any flag in a group AND then clear
//                                  the flags that are present, set 'wait_type' to:
//                                  OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME
                      INT32U        timeout,// =0 表示一直等待下去
                      INT8U        *perr)

OS_FLAGS  OSFlagPost (OS_FLAG_GRP  *pgrp,
                      OS_FLAGS      flags,
                      INT8U         opt,
                      INT8U        *perr)

```

21

消息邮箱
```C
OS_EVENT  *OSMboxCreate (void *pmsg)//创建消息邮箱
void  *OSMboxPend (OS_EVENT  *pevent,
                   INT32U     timeout,
                   INT8U     *perr)//等待消息邮箱
//returnS:    != (void *)0  is a pointer to the message received
//*           == (void *)0  if no message was received
INT8U  OSMboxPost (OS_EVENT  *pevent,
                   void      *pmsg)//发送消息邮箱

```

22

消息队列Queue:很多消息邮箱按一定顺序排列.先进先出 后进先出
```C
OS_EVENT  *OSQCreate (void    **start,//指向数组地址的指针 双指针
                      INT16U    size)

INT8U  OSQPost (OS_EVENT  *pevent,//指向消息队列创建的事件
                void      *pmsg)//发送的消息

void  *OSQPend (OS_EVENT  *pevent,
                INT32U     timeout,//=0表示死等待
                INT8U     *perr)
//Returns: 返回值是一个指向收到信息的地址(指针)

```

23
内存申请:void* malloc(sizeof());//申请一定空间的内存 返回该空间的首地址
从内存条的`堆`中申请内存
内存释放:free
用户使用malloc和free会产生内存碎片
uCOS为了解决内存碎片问题 使用块机制(block)
要使用内存块,需要用OSMemCreate()创造内存块 交给uCOS来管理
```C
//创建一个分区
OS_MEM  *OSMemCreate (void  *addr,//首地址
                      INT32U  nblks,//划分块数
                      INT32U  blksize,//每一块大小
                      INT8U  *perr)//错误码

//申请一个内存块
void  *OSMemGet (OS_MEM  *pmem,//分区名
                 INT8U   *perr)//错误码
//返回首地址

//释放一个内存块
INT8U  OSMemPut (OS_MEM  *pmem,//分区名
                 void    *pblk)//内存块地址
```

24static

静态局部变量:生存周期是整个源程序,但是只能在该函数内使用
静态全局变量: 非静态全局 变量的作用域是整个源程序， 当一个源程序由多个源文件组成时，非静态的全局变量在各个源文件中都是有效的。 而静态全局变量则限制了其作用域， 即只在 定义该变量的源文件内有效， 在同一源程序的其它源文件中不能使用它。由于静态全局变量的作用域局限于一个源文件内，只能为该源文件内的函数公用， 因此 可以避免在其它源文件中引起错误。

25

指针数组：首先它是一个数组，数组的元素都是指针，数组占多少个字节由数组本身决定。它是“储存指针的数组”的简称。
数组指针：首先它是一个指针，它指向一个数组。在32 位系统下永远是占4 个字节，至于它指向的数组占多少字节，不知道。它是“指向数组的指针”的简称。

26

栈区（stack）— 由编译器自动分配释放 ，存放函数的参数值，局部变量的值等。其操作方式类似于数据结构中的栈。
堆区（heap） — 一般由程序员分配释放， 若程序员不释放，程序结束时可能由OS回收 。

申请方式
stack:
由系统自动分配。 例如，声明在函数中一个局部变量 int b; 系统自动在栈中为b开辟空间
heap:
需要程序员自己申请，并指明大小，在c中malloc函数
如p1 = (char *)malloc(10);

申请大小的限制
栈：在Windows下,栈是向低地址扩展的数据结构，是一块连续的内存的区域。这句话的意思是栈顶的地址和栈的最大容量是系统预先规定好的，在WINDOWS下，栈的大小是2M（也有的说是1M，总之是一个编译时就确定的常数），如果申请的空间超过栈的剩余空间时，将提示overflow。因此，能从栈获得的空间较小。
堆：堆是向高地址扩展的数据结构，是不连续的内存区域。这是由于系统是用链表来存储的空闲内存地址的，自然是不连续的，而链表的遍历方向是由低地址向高地址。堆的大小受限于计算机系统中有效的虚拟内存。由此可见，堆获得的空间比较灵活，也比较大。

27定时器

uCOS提供的是软件定时器.
好处: 使用一个硬件定时器来产生很多定时器 无上限.使用方便,由操作系统管理.
缺点:精度不高 定时时间稍长

```C
OS_TMR  *OSTmrCreate (INT32U           dly,//启动多少时间后开始运行
                      INT32U           period,//周期
                      INT8U            opt,//选项 一次定时器 还是 周期定时器
                      OS_TMR_CALLBACK  callback,//回调函数
                      void            *callback_arg,
                      INT8U           *pname,
                      INT8U           *perr)//定时器创建

#define OS_TMR_CFG_WHEEL_SIZE     8u   /*     Size of timer wheel (#Spokes)   */ //定时器轮转查询是否到达设定时间的轮转个数
```
回调函数: 系统反过来调用应用程序的代码

29移植

与处理器有关的需要移植的代码:uC/OS-II Ports目录 内核相关文件
os_cpu_c.c, os_cpu_a.asm, os_cpu.h, os_dbg_r.c, os_dbg.c
还有板级支持包 包括启动代码 库函数 如 startup_stm32f40_41xxx.s

移植时 UCOSII/CORE中的ucos_ii.c不用添加 该文件重新定义了数据类型别名 避免重复定义 产生冲突
关于debug相关的都不需要添加

关于os_core.c:
OSInitHookBegin();钩子函数 附带在某个任务执行中的函数 一般为空
OSSchedLock();调度器上锁
一般core文件中的函数不需要修改和调用

关于os_cpu.h:
一般需要修改的是栈的增长方向,和进入退出临界段代码的实现方法
//OS_CRITICAL_METHOD = 1 :直接使用处理器的开关中断指令来实现宏
//OS_CRITICAL_METHOD = 2 :利用堆栈保存和恢复CPU的状态
//OS_CRITICAL_METHOD = 3 :利用编译器扩展功能获得程序状态字，保存在局部变量cpu_sr

关于os_cfg.h:
以_EN结尾的都是开关量,1打开,0关闭
```C
#define OS_TICKS_PER_SEC       1000u   /* Set the number of ticks in one second                        */
```
每秒调度1000次 即每1ms调度一次,实时性高 但是系统无用功多 需要综合考虑






####不可剥夺型内核
优点: 响应中断快,可以调用不可重入函数, 不用等待其他任务可能在使用这个函数. 另一个优点是几乎无需使用信号量保护数据.
最大的缺陷在于其响应时间.高优先级任务即使已经就绪, 也要等待当前任务完成并释放CPU. 因此任务级响应时间不确定.
因此大多数实时操作系统都是可剥夺型的.

####时间片轮番调度法
当两个或两个以上任务有同样的优先级时,内核允许1个任务运行事先确定的一段时间. 这段时间叫做时间额度,然后切换给另一个任务. uCOS不支持时间片轮番调度法. 应用程序中各任务的优先级必须互不相同.uCOS支持动态优先级.

####优先级反转问题
优先级顺序: 任务1 > 任务2 > 任务3
假设任务3在运行, 这个时候任务1等待的事件到来, 要使用CPU, 但是假设任务3正在使用一个任务1也要使用的资源, 任务1就要等待任务3使用完此资源. 假设此时, 任务2等待的事件到来, 也不需要被任务3占用的资源. 那么任务3被打断, 任务2开始执行. 任务2执行完后把控制权交还任务3, 任务3释放资源后, 任务1才能执行. 这样,任务2虽然优先级低于任务1,但是比任务1先执行. 称为优先级反转.
解决方法: 在任务3使用共享资源时,提高任务3的优先级, 完成后恢复优先级. 


####互斥条件
实现任务间通信的简单方法是使用共享数据结构. 而处理共享数据时保持互斥最好的办法就是开关中断. 
或者使用信号量(semaphore)

信号量:
对信号量只能执行3种操作: initialize(create), wait(pend), signal(post 发送).收到信号量的任务可能是:
`(1)` 等待信号量中优先级最高的.
`(2)` 最早开始等待信号量的任务.按FIFO原则.
有些内核具有选择项, 允许在信号量初始化时,选定其中一种. 但uCOS只支持优先级法. 
处理简单的共享变量也使用信号量是多余的. 可能只需要通过开关中断. 

####开关中断
uCOS定义2个宏来开关中断. `OS_ENTER_CRITICAL()`和`OS_EXIT_CRITICAL()`. 中间可以插入`临界段`代码.可以在对应型号的微处理器的`OS_CPU.H`中找到. 调用uCOS功能函数时, 中断总应当是开着的.


####任务状态
睡眠态:任务驻留在程序空间, 还没有交给uCOS来管理. 需要用`TaskCreate()` `TaskCreateExt()`来实现的.
就绪态:任务一旦建立,该任务就进人就绪态, 准备运行.   一个任务可以通过调用`TaskDel()`返回到睡眠态. 
运行态:调用`OSStart()`可以启动多任务.              
等待状态: 正在运行的任务可以通过调用`OSTimeDly()`和`TimeDlyHMSM()`, 将自身延迟一段时间,进入等待状态, 直到函数中定义的延迟时间到. 
中断服务态: 被中断的任务进入中断服务态. 

####任务调度
任务级的调度由函数OSSched()完成. 中断级调度由OSIntExt()完成. 
任务调度过程: 将被挂起任务的处理器寄存器推入堆栈, 然后将较高优先级任务的寄存器值从栈中恢复到寄存器中. 
```c++
INT8U OSTaskCreate(void(* task)(void *pd),void *pdata, OS_STK *ptos, INT8U prio);// 任务建立
```

####任务堆栈
可以用`malloc()`函数动态地分配堆栈空间. 在动态分配中,要注意内存碎片问题. 特别是当反复地建立和删除任务时, 内存堆中可能会有大量的内存碎片. 导致没有足够大的一块连续内存区域可用作任务堆栈. 这时, `malloc()`便无法成功地为任务分配堆栈空间. 
有的CPU的堆栈是向上递增的,有的是向下递减的. 通过OS_CPU.H文件中的OS_STK_GROWTH控制. 
对递增和递减的堆栈都提供支持的代码:
```C++
OS_STK TaskStack[TASK_STACK_SIZE];

#if OS_STK_GROWTH == 0 //递增
  OSTaskCreate(task, pdata, &TaskStack[0],prio);

#else
  OSTaskCreate(task, pdata, &TaskStack[Task_STACK_SIZE-1],prio);

#endif

```
####堆栈校验
为了确定实际需要的堆栈空间大小. 为了使用这个功能, 必须满足:
- `OS_FDG.H`文件中置`OS_TASK_CREATE_EXT`为1
- 用`OSTaskCreateExt()`建立任务, 将参数`opt`设置为`OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR`.
- 把需要进行堆栈校验的任务的优先级作为`OSTaskStkChk()`的参数并调用. 可以检验任何任务的堆栈, 并不是只能检验正在运行的任务. 


####存储
SRAM: 静态RAM 价格很高 所需面积大 不需要定时刷新 用于CPU一二级cache
DRAM: 动态RAM 价格较低 所需面积小 需要定时刷新 用于内存
SDRAM: Synchronous Dynamic Random Access Memory，同步动态随机存储器. DDR RAM是其中的一种.
EEPROM是通过 电子擦出，价格很高，写入时间很长，写入很慢。
FLASH存储器又称闪存，它结合了ROM和RAM的长处，不仅具备电子可擦除可编程（EEPROM）的性能，还不会断电丢失数据同时可以快速读取数据 （NVRAM的优势），U盘和MP3里用的就是这种存储器。在过去的20年里，嵌入式系统一直使用ROM（EPROM）作为它们的存储设备，然而近年来 Flash全面代替了ROM（EPROM）在嵌入式系统中的地位，用作存储Bootloader以及操作系统或者程序代码或者直接当硬盘使用（U盘）。
Flash目前主要是两种: NAND Flash 和 NOR Flash
NOR是现在市场上主要的非易失闪存技术。NOR一般只用来存储少量的代码；NOR主要应用在代码存储介质中。NOR的特点是应用简单、无需专门的 接口电路、传输效率高，它是属于芯片内执行(XIP, eXecute In Place)，这样应用程序可以直接在（NOR型）flash闪存内运行，不必再把代码读到系统RAM中。在1～4MB的小容量时具有很高的成本效益，但 是很低的写入和擦除速度大大影响了它的性能。NOR flash带有SRAM接口，有足够的地址引脚来寻址，可以很容易地存取其内部的每一个字节。NOR flash占据了容量为1～16MB闪存市场的大部分。
NAND结构能提供极高的单元密度，可以达到高存储密度，并且写入和擦除的速度也很快。应用NAND的困难在于flash的管理和需要特殊的系统接口。


####任务划分与设计
- 对于`关键`功能,必须尽可能与其他功能剥离,独立成为一个任务. 通过通信方式再触发其他任务, 完成后续操作. 
- 紧迫性是指某种功能必须在规定的时间内得到运行权, 并在规定的时刻前执行完毕, 这类功能对实时性有严格要求. 

采用创建任务的方式来启动一个任务, 除了实时性差以外, 还可能在任务自我删除之前出现问题. 比如如果使用了共享资源, 则必须在自我删除前释放. 如果该任务有关联任务, 需要在删除前将关联解除. 解除关联又需要删除关联任务和通信工具, 是很麻烦的事情. 
因此,适合采用创建任务的方式来启动的任务, 通常是孤立任务.不和其他任务进行通信, 只使用共享资源来获取信息和输出信息. 
事件触发执行的任务函数中的`调用获取事件的函数`,使用了操作系统提供的某种通信机制,等待另外一个任务(或ISR)发出的信息(如信号量或邮箱中的消息)
有些触发任务的事件属于`信号类`(信号量), 其作用仅仅是启动任务的运行. 还有一些触发任务的事件属于`信息类`,其作用不仅仅是启动任务, 而且为该任务提供原始数据和资料. 
uCOS保留对最高的四个优先级(0,1,2,3)和最低的四个优先级(`OS_LOWEST_PRIO-3`,`OS_LOWEST_PRIO-2`,`OS_LOWEST_PRIO-1`,`OS_LOWEST_PRIO`)的使用权, 以备将来操作系统升级时使用. 


####中断设计
快速中断请求（Fast Interrupt Request，FIQ):在ARM中，FIQ模式是特权模式中的一种，同时也属于异常模式一类。用于高速数据传输或通道处理，在触发快速中断请求（FIQ）时进入。
IRQ(Interrupt Request) 中断请求

`#define KEY (1 << 20)` 表示第20位被设置为1  
(Markdown 换行: 两个空格后回车)   
为了使用ISR汇编宏, 每个受操作系统管理的ISR都必须按汇编宏要求的格式, 在文件IRQ.S的尾部添加中断句柄.
```Assembly
XXX_Handler HANDLER XXXX_Exception
```
`XXX_Handler` 是ISR的起始地址, 即汇编宏的起始地址. 在初始化向量中断控制器时作为中断向量地址使用.    
`HANDLER`是句柄关键词, 必须大写  
`XXXX_Exception`是用户用C编写的功能函数名, 该函数供汇编宏调用.      
例如,要使用定时器1作为中断源, 需要在文件IRQ.S的尾部添加:
```Assembly
Timer1_Handler HANDLER Timer1_Exception
```
在一个中断源开始工作之前, 需要配置和初始化中断源, 使它按预定的参数和方式工作. 
- 配置中断源工作参数: 通过寄存器,设置定时器工作方式, 使能等.  
- 向量中断控制器配置: 使中断信号和对应的ISR联系起来, 必须对向量中断控制器进行配置. 设中断源XXXX的通道号为T, 配置中断优先级为Y, 则需要配置:

``` C++
extern void XXXX_Handle(void);		//声明中断源XXXX的中断服务函数ISR
VICVectAddrY = (INT32U)XXXX_Handler;	//将ISR入口地址填入向量寄存器Y
VICVectCntlY = (0x20 | T);			//向量中断方式, 通道号T
```
以定时器1(通道号为5)中断为例, 配置优先级Y=2时: 
```C++
extern void Timer1_Handler(void);	//声明定时器1的中断服务函数
VICVectAddrY = (INT32U)Timer1_Handler;
VICVectCntlY = (0x20 |0x05);

//控制中断开关
VICIntEnable = 1 << 5;		//使能
VICIntEnClr = 1 << 5;		//禁止
```
ISR与关联任务的通信方式有两种基本类型: 信号(信号量)型和数据(消息)型. 信号量只是一个触发, 数据消息型可以传送数据.


二值信号量不能处理信号积压现象. 未及时响应的信号将会被遗弃. 使有效响应次数少于实际发生的信号次数.  

在这种情况下, 采用计数信号量是一个有效的选择. 在总体上能够使控制方对被控制方进行同步控制. 在特殊情况下也能够不失去控制. 保证每次控制信号都能够得到响应.   
使用范围: 被控制方不能保证在下一次信号到来之前处理完本次控制方发出的信号, 但总体上可以响应所有信号. 

事件标志组: 是若干二值信号的组合, 其中每一个二值信号都是某个事件是否发生的标志. 可以实现多个任务协同控制一个任务,当各个相关任务先后发出自己的信号后, 预定的逻辑运算结果有效, 触发被控制的任务.   
事件标志组的使用很灵活, 可以将标志定义为1有效或者0有效. 可以为逻辑`AND`(所有标志位均有效)或者逻辑`OR`(只要有一个有效). 

消息邮箱: 控制方在对被控制方进行控制时, 还需要向被控制方提供内容信息(数据,字符串)时, "消息邮箱"是一种有效方案.   
消息邮箱中只能存放一条消息, 所以使用时, 必须满足:任何时候消息的产生速度都比消息的消费速度慢, 即被控制任务总是在等待消息. 类似于二值信号量.

消息队列: 可以存放多个消息, 能够解决消息的临时堆积问题. 类似计数信号量. 

总结:同步过程不需要传输具体内容时, 选择信号量类手段(二值,计数,事件标志组). 当同步过程需要传输具体内容时, 选择消息类手段(消息邮箱和消息队列).
  
  
####回调函数
回调函数实际上就是在调用某个函数（通常是API函数）时，将自己的一个函数（这个函数为回调函数）的地址(函数指针)作为参数传递给那个函数。而那个函数在需要的时候，利用传递的地址调用回调函数，这时你可以利用这个机会在回调函数(自己写的这个函数)中处理消息或完成一定的操作。
钩子函数指钩子在拦截了消息后，进行对应消息处理的函数，可以通过返回TRUE直接抛弃消息，其原型为：
LRESULT CALLBACK HookProcName(int nCode ,WPARAM wParam,LPARAM lParam)
HHOOK -- 钩子句柄，需要保留，等不使用钩子时通过UnhookWindowsHookEx函数卸载钩子  
参数说明：  
idHook -- 钩子的拦截消息类型，选择钩子程序的拦截范围，具体值参考文章结尾的消息类型  
lpfn -- 消息的回调函数地址，一般是填函数名  
hMod -- 钩子函数所在的实例的句柄。对于线程钩子，该参数为NULL；对于系统钩子，该参数为钩子函数所在的DLL句柄。在dll中可通过AfxInitExtensionModule(MousehookDLL, hInstance)获得DLL句柄。  
dwThreadId --钩子所监视的线程的线程号，可通过GetCurrentThreadId()获得线程号。对于全局钩子，该参数为NULL(或0)。  
  
  
####行为同步
- 两个任务之间的单向同步: 
	(1) 控制方任务优先级 < 被控制方任务优先级:   控制方任务发出消息后使被控制方任务进入就绪状态, 并立即发生任务切换. 使被控制方任务直接进入运行状态. 瞬时同步效果好. 
	(2) 控制方任务优先级 > 被控制方任务优先级(大多数情况):  
	控制方任务发出信息后虽然使被控制方任务进入就绪状态,但并不发生任务切换. 只有当控制方使自己挂起时, 被控制方任务才会运行.   
常用做法是采用首尾出发方式(控制方任务的尾部出发被控制方的首部).控制方在发出信息后调用延时函数, 主动使自己挂起, 让低优先级的被控制方任务尽快得到运行机会. 从而改善同步效果.  

- 两个任务之间的双向同步:
需要连个以上任务来同步一个任务时, 采用事件标志组来实现. 
```C
/*--!--只是部分骨架代码--!--*/

void TaskLed(void *pdata);
void TaskBeep(void *pdata);
void TaskKey(void *pdata);

OS_FLAG_GRP 	*FLAGW; //定义事件标志组

int main()
{
	OSInit();
	OSTaskCreate(TaskLed, (void *)0, &TaskStkLed[TaskStk-1],5);//TaskLed是等待任务, 优先级最低 被同步任务
	OSStart();
	return 0;
}

void TaskLed(void *pdata)
{
	FLAGW = OSFlagCreate(0,&err);	//创建标志组
	OSTaskCreate(TaskBeep, (void *)0, &TaskStkBeep[TaskStk-1],3);//创建蜂鸣器任务,优先级最高
	OSTaskCreate(TaskKey, (void *)0, &TaskStkKey[TaskStk-1],4);//创建蜂鸣器任务,优先级中

	while(1)
	{
		OSFlagPend(FLAGW,	//等待标志组
				   0x03,	//最低两位
				   OS_FLAG_WAIT_SET_ALL + //是否全为1
				   OS_FLAG_CONSUME,		//获取成功后清零
				   0, &err);
		i++;
	}
}

void TaskBeep(void *pdata)
{
	while(1)
	{
	...//蜂鸣器鸣响
	OSFlagPost(FLAGW,		//等待标志组
			   0x01,			//最低位
			   OS_FLAG_SET,		//置1
			   &err);
	}

}

void TaskKey(void *pdata)
{	
	while(1)
	{
		...//按键检测
		//按下释放后
		OSFlagPost(FLAGW,		//等待标志组
				   0x02,		//次低位
				   OS_FLAG_SET,		//置1
				   &err);

	}

}
```
被同步任务的执行次数不超过各个同步任务中发出信号量最少的执行次数. 在上例中, 按键次数多于蜂鸣器鸣响的次数时,显示数据等于蜂鸣器响声次数. `逻辑与`的控制功能具有安全控制的特点. 当逻辑关系为逻辑或时,只要其中有一个关联任务将对应的标志位置1, 就会触发被控制的任务.   
- 多任务相互同步
多任务相互同步可以将若干相关任务的运行频度保持一致. 事件标志组可以用来进行多任务相互同步: 每个相关任务运行到同步点时都要签到, 即调用发送标志函数将对应标志置位, 然后调用获取标志组函数进入等待状态. 


####资源同步
使得不同人物访问共享资源时能够确保共享资源信息可靠和完整的措施称为资源同步.  
资源同步有关中断,关调度,使用互斥信号量和使用计数信号量几种方法.   
但是也不是所有共享资源都需要采取资源同步措施, 有类共享资源具有只读特性, 信息只能读出,不能写入.  
关调度的方法没有将中断关闭, 故系统对各种异步事件仍然可以及时响应, 并使相关任务进入就绪状态.但不会立即进入运行状态. 缺点是: 使与该资源无关的任务受到牵连. 即使它们的优先级足够高并被ISR出发到就绪状态也无法运行, 而这类与ISR关联的任务通常有比较高的实时性要求. 
开关调度: `OSSchedLock()`和`OSSchedUnlock()`
关调度的资源同步方法优点不多,缺点不少, 尽可能不要使用. 最好用互斥信号量来进行资源同步. 




