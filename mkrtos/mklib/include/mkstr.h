#ifndef _MK_STR_H_
#define _MK_STR_H_

#include <mkinc.h>

MK_RTOS_EXT mk_int32_t mk_strcpy(char* _dst, const char* _src);
MK_RTOS_EXT void mk_memset(void *_dst,const char ch,mk_size_t _size);

#endif
