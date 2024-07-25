#ifndef MKRTOSCONFIG_H
#define MKRTOSCONFIG_H

//调试台 1：表示使用系统调试 0：表示不使用
#define MK_USING_SYS_DEBUG 1

//表示是否使用qmenu
#define MK_USE_QMENU_DEBUG 1

//优先级个数 >= 32
#define MK_TASK_PRIORITY_NUM 32

//栈增长方向 1向上 0向下
#define MK_STACK_DIRECTION 0

//调试台的缓存大小
#define MK_DEBUG_BUFF_SIZE 256

//芯片频率
#define MK_SYSTEM_MCU_CLK 72000000UL

//main任务栈大小
#define MK_MAIN_STACK_SIZE 512

//main 任务优先级 
#define MK_MAIN_TASK_PRIORITY 5

//任务名字长度
#define MK_TASK_NAME_LEN 16

//idle 空闲任务栈大小
#define MK_IDLE_STACK_SIZE 512

//idle 空闲优先级 
#define MK_IDLE_TASK_PRIORITY MK_TASK_PRIORITY_NUM - 1

//通讯变量名字长度
#define MK_COM_NAME_LEN 32

//信号量
#define MK_USE_SEMAPHORE 1

//通讯变量名字长度
#define MK_EVENT_NAME_LEN 32

//事件
#define MK_USE_EVENT 1

//软定时器
#define MK_USE_SOFTTIMER 1

//定时器名字长度
#define MK_SOFTTIMER_NAME_LEN 16

//软定时器任务栈大小
#define MK_SOFTTIMER_STACK_SIZE 256

//软定时器任务优先级 
#define MK_SOFTTIMER_TASK_PRIORITY 1

#endif
