#include <mkipc.h>

#if MK_USE_IPC

/**
 * @brief 初始化 ipc 父对象
 * @param _obj
 * @param _name
 * @return int
 */
int mk_ipc_init(mk_object_t* _obj, const char* _name) {
    mk_init_list(_obj->list);

    mk_memset(_obj->name, 0, sizeof(_obj->name));
    mk_strcpy(_obj->name, _name);

    return 0;
}

#endif