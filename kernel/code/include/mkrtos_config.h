#ifndef MKRTOS_CONFIG_H
#define MKRTOS_CONFIG_H

//时间戳
#define USE_TIME_STAMP 0 //关闭

//时间片
#define USE_TIME_SLICE 1 //开启 不开启同一个优先级的任务只能就绪列表的第一个才能运行

//支持最大的优先级
#define MK_PRIORITY_MAX	32

//线程名字长度
#define MK_TASK_NAME_LEN 8

//时基列表长度
#define MK_TICK_WHEEL_SIZE 16

//是否启用挂起功能
#define USE_SUSPEND_CTRL 1 //启用

//main函数的优先级
#define MAIN_TASK_PRIORITY MK_PRIORITY_MAX/2

#define MAIN_TASK_STACK_SIZE 512

#endif
