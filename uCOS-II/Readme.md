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
