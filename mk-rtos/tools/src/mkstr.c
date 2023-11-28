#include <mkstr.h>

mk_int32_t mk_strncpy(char * _drc,const char *_src, mk_uint32_t _len)
{
	mk_int32_t i = 0;
	if(_drc==MK_NULL || _src == MK_NULL){
		return -1;
	}
	
	while((*(_drc++)=(*_src++)!='\0')&&((++i)<_len));
	
	return i;
}

mk_int32_t mk_strlen(const char *_src)
{
	mk_int32_t i = 0;
	if(_src == MK_NULL){
		return -1;
	}
	while(*(_src+(i++)) != '\0');
	return i;
}