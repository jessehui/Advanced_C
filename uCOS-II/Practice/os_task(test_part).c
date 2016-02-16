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



//**************************************************
//			TASK STACK DATA
#if OS_TASK_CREATE_EXT_EN > 0u
typedef struct os_stk_data{
	INT32U	OSFREE;//number of free bytes on the stack
	INT32U 	OSUSED;//number of used bytes on the stack
}OS_STK_DATA		
#endif

//**************************************************
//			STACK CHECKING
//this function is called to check the amount of free memory left
//on the specified task's stack
//Arguments: prio   	--  task priority
//	     p_stk_data --  is a pointer to a data structure of type OS_STK_DATA
INT8U OSTaskStkChk(	INT8U prio,
			OS_STK_DATA *p_stk_data
)

// //**************************************************
// 			OS TASK DELETE
// Auguments: prio 	-- priority of the task to delete. Note that you can
// 			 explicitly delete the current task without knowing the priority
// 			 level by setting prio to OS_PRIO_SELF
#if OS_TASK_DEL_EN > 0u
INT8U OSTaskDel(INT8U prio)
{

}

// *******************************************************
// 		Task Delete Request
// Arguments:  prio 	
// Return: 
// 	OS_ERR_NONE			no error.task existed and request registered
// 	OS_ERR_TASK_NOT_EXIST  		Task has been deleted.
// 	OS_ERR_TASK_DEL			if the task is assigned to a mutex(互斥体)
// 	OS_ERR_TASK_DEL_IDLE		if you requested to delete uCOS's idle task
// 	OS_ERR_PRIO_INVALID		invalid priority
// 	OS_ERR_TASK_DEL_REQ		if a task request that the running task be deleted
#if OS_TASK_DEL_EN > 0u
INT8U OSTaskDelReq(INT8U prio)

// *******************************************************
// 		Task Name set
// Arguments: perr -- a pointer to an error code that contains one of the following values
		
// 		OS_ERR_NONE 		if the requested task is resumed
// 		OS_ERR_TASK_NOT_EXIST	if task is not created or assigned to a mutex
// 		OS_ERR_TASK_NAME_TOO_LONG 
// 		OS_ERR_PNAME_NULL	passed a wrong pointer to pname
// 		OS_ERR_PRIO_INVALID	
// 		OS_ERR_NAME_SET_ISR	If you called this function from an ISR(interrupt service request)


#if OS_TASK_NAME_EN > 0u
void OSTaskNameSet(INT8U prio,
		   INT8U *pname,//指向任务名字的指针
		   INT8U *perr)//错误码指针

// *******************************************************
// 		Task Name get
//Return: the length of the name string or 0 if the task doesn't exist
#if OS_TASK_NAME_EN > 0u
INT8U OSTaskNameGet(INT8U prio,
		    INT8U **pname,//双指针 指向指针的指针
		    INT8U *perr）//错误码指针

// *******************************************************
// 		Task Suspend
//Returns:
		 // OS_ERR_NONE
		 // OS_ERR_TASK_SUSPEND_IDLE  you can't suspend an idle task
		 // OS_ERR_PRIO_INVALID
		 // OS_ERR_TASK_SUSPEND_PRIO  if the task to suspend doesn't exist
		 // OS_ERR_TASK_NOT_EXIST	   if the task is assigned to a mutex 
#if OS_TASK_SUSPEND_EN > 0u
INT8U OSTaskSuspend(INT8U prio)	
{}

// *******************************************************
// 			Time Delay
void OSTimeDly(INT32U ticks)
{}

// *******************************************************
// 	Time Delay Hour Minute Second Millisecond
#if OS_TIME_DLY_HMSM_EN > 0u
void OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U ms)


