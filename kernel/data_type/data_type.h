#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#ifdef	MK_RTOS_GLOBALS
	#define	MK_RTOS_EXT
#else
	#define	MK_RTOS_EXT	extern
#endif

#define MK_NULL (void *)0
	
//RTOS的自定义数据类型
typedef unsigned int   mk_uint32;
typedef unsigned short mk_uint16;
typedef unsigned char  mk_uint8;

typedef int   mk_int32;
typedef short mk_int16;
typedef char  mk_int8;
	
typedef mk_uint32 mk_taskstack;


#endif
