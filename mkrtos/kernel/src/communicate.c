#include <mkcommunicate.h>
#include <mkdebug.h>
#include <mklist.h>
#include <mkstr.h>
#include <mktask.h>

#if MK_USE_SEM

/**
 * @brief 创建信号量
 * @param _name
 * @param _sem_value
 * @return mk_sem_t
 */
mk_sem_t mk_sem_create(const char* _name, mk_size_t _sem_value) {
    mk_sem_t _sem;
    _sem.obj.is_use = MK_FALSE;

    mk_init_list(&_sem.obj.sem_list);

    mk_memset(_sem.obj.name, 0, sizeof(_sem.obj.name));
    mk_strcpy(_sem.obj.name, _name);

    _sem.base_value = _sem_value;
    _sem.value = _sem_value;

    return _sem;
}

/**
 * @brief 释放信号量
 * @param _sem
 * @return mk_code_t
 */
mk_code_t mk_sem_release(mk_sem_t* _sem) {
    mk_task_t* _tmp_task = MK_NULL;
    mk_uint32_t _status;
    _status = mk_enter_critical();
    _sem->obj.is_use = MK_TRUE;
    if (_sem->value < _sem->base_value && _sem->obj.sem_list.task_num > 0) {
        _sem->value++;

        // 获取头部节点
        _tmp_task = mk_get_node_from_list(&_sem->obj.sem_list);
        if (_tmp_task != MK_NULL) {
            // 将任务从信号量列表移除
            mk_delete_node_from_list(&_sem->obj.sem_list, _tmp_task, GET_STR_DATA_ADDR_OFFSET(mk_task_t, sem_list));
            // 插入就绪队列
            mk_insert_node_to_ready_list(_tmp_task);
        }
    }

    mk_exit_critical(_status);

    return MK_SUCCESS;
}

/**
 * @brief 获取信号量
 * @param _sem
 * @return mk_code_t
 */
mk_code_t mk_sem_get(mk_sem_t* _sem) {
    mk_uint32_t _status;
    _status = mk_enter_critical();
    if (_sem->value > 0) {
        _sem->value--;
        mk_exit_critical(_status);
        goto exit;
    }

    g_current_task->task_status = MK_TASK_STATUS_SUSPEND;

    // 插入信号量队列
    mk_insert_node_to_list_tail(&_sem->obj.sem_list, g_current_task, GET_STR_DATA_ADDR_OFFSET(mk_task_t, sem_list));

    // 从就绪队列移除
    mk_delete_node_from_ready_list(g_current_task);

    mk_exit_critical(_status);

    // 进行任务调度
    mk_tack_scheduler();

    return MK_BUSY;
exit:

    return MK_SUCCESS;
}

/**
 * @brief 打印信号列表
 * @param _sem
 */
void mk_printf_sem_list(mk_sem_t* _sem) {
    mk_uint32_t _status;
    _status = mk_enter_critical();
    mk_task_t* tmp = _sem->obj.sem_list.next;
    mkprintk("#########################################\r\n");
    mkprintk("#### print sem list\r\n");
    mkprintk("#### sem name:%s\r\n", _sem->obj.name);
    mkprintk("#### task all num:%d\r\n", _sem->obj.sem_list.task_num);
    while (tmp != MK_NULL) {
        mkprintk("#### \ttask name:%s prio:%d delay:%d\r\n", tmp->task_name, tmp->prio, tmp->delay_systick);
        tmp = tmp->ready_list.next;
    }
    mkprintk("#########################################\r\n");
    mk_exit_critical(_status);
}

#endif
