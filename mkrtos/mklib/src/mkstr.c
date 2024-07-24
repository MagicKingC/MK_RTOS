#include <mkstr.h>

/**
 * @brief 字符串拷贝
 * @param _dst 
 * @param _src 
 * @return mk_int32_t 拷贝的字符串长度
 */
mk_int32_t mk_strcpy(char* _dst, const char* _src) {
    mk_int32_t len=0;
    while (_src[len] != '\0')
    {
        _dst[len] = _src[len];
        len++;
    }
    
    if (len > 0)
    {
        len++;
    }
    
    return len;
}

/**
 * @brief  往某一块空间写入字符ch
 * @param _dst 
 * @param ch 
 * @param _size 
 */
void mk_memset(void *_dst,const char _ch,mk_size_t _size){
    while (_size >= 0)
    {
        ((char *)_dst)[--_size] = _ch;
    }
}
