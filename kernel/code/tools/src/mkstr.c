#include <mkstr.h>

mk_int32 mk_strncpy(char * drc,const char *src,mk_uint32 len)
{
	mk_uint32 i = 0;
	if(drc==MK_NULL || src == MK_NULL){
		return -1;
	}
	
	while((*(drc++)=(*src++)!='\0')&&((++i)<len));
	
	return i;
}

mk_int32 mk_strlen(const char *src){
	mk_uint32 i = 0;
	if(src == MK_NULL){
		return -1;
	}
	while(*(src+(i++)) != '\0');
	return i;
}