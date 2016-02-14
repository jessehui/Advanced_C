INT8U OSTaskCreate( 	void (*task)(void *p_arg),//函数指针 task是函数地址，*p_arg为参数。
			//整个参数的意义是指向任务起始位置的指针
			void *p_arg,//传递给任务的参数 一般为NULL
			OS_STK *ptos,//指向任务堆栈栈顶的指针。OS_STK是任务堆栈的数据类型
			INT8U prio//任务优先级
		 )

#if OS_TASK_CREATE_EXT_EN > 0u
INT8U OSTaskCreateExt(	void (*task)(void *p_arg),//函数指针 指向任务的首地址（就是任务的名称）
			void *p_arg,//传递给任务第一次运行的参数 一般为空指针，(void*) 0
			OS_STK *ptos,//任务堆栈栈顶指针 最后一个元素的指针
			INT8U prio,//优先级
			INT16U ID,//任务ID号 设置为和优先级相同
			OS_STK *ptos,//任务堆栈栈底指针 第0个元素的地址
			INT32U stk_size,//任务堆栈大小
			void *pext,// 传递给任务额外的参数 大多数也设置为空指针
			INT16U opt //选项
		）