#ifndef DATA_TYPE_H
#define DATA_TYPE_H


#ifdef	MK_RTOS_GLOBALS
	#define	MK_RTOS_EXT
#else
	#define	MK_RTOS_EXT	extern
#endif

#define MK_NULL (void *)0
#define MK_TRUE	 1
#define MK_FALSE 0

enum _MK_CODE_{
	MK_FAIL = -1,
	MK_SUCCESS = 0,
};
	
//RTOS的自定义数据类型
typedef unsigned int   mk_uint32_t;
typedef unsigned short mk_uint16_t;
typedef unsigned char  mk_uint8_t;

typedef int   mk_int32_t;
typedef short mk_int16_t;
typedef char  mk_int8_t;

	
typedef mk_uint32_t mk_TaskStack;
typedef mk_uint32_t mk_TaskStatus;
typedef mk_uint32_t mk_TaskTime;
typedef mk_uint32_t mk_TaskTick;
typedef mk_uint8_t  mk_TaskPrio;


typedef mk_uint8_t mk_bool_t;
typedef enum _MK_CODE_  mk_code_t;


#define __weak __attribute__((weak))


#endif
