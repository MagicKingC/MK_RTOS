#include <mkdebug.h>
#include <mklist.h>
#include <mksemaphore.h>
#include <mkstr.h>
#include <mktask.h>

#if MK_USE_SEMAPHORE

/**
 * @brief 创建信号量
 * @param _sem
 * @param _name
 * @param _sem_value
 * @return mk_code_t
 */
mk_code_t mk_sem_create(mk_sem_t* _sem, const char* _name, mk_size_t _sem_value) {
    _sem->is_use = MK_FALSE;

    MK_ASSERT(_sem == MK_NULL, "_sem is null");

    mk_ipc_init(&_sem->obj, _name);

    _sem->value = _sem_value;

    return MK_SUCCESS;
}

/**
 * @brief 释放信号量
 * @param _sem
 * @return mk_code_t
 */
mk_code_t mk_sem_release(mk_sem_t* _sem) {
    mk_task_t* _tmp_task = MK_NULL;
    mk_uint32_t _status;

    MK_ASSERT(_sem == MK_NULL, "_sem is null");

    _status = mk_enter_critical();
    _sem->is_use = MK_TRUE;
    if (_sem->obj.list.task_num > 0) {
        // 获取头部节点
        _tmp_task = mk_get_node_from_list(&_sem->obj.list);
        if (_tmp_task != MK_NULL) {
            // 将任务从信号量列表移除
            mk_delete_node_from_list(&_sem->obj.list, _tmp_task, GET_STR_DATA_ADDR_OFFSET(mk_task_t, sem_list));
            // 插入就绪队列 立马执行所以无需增加一个信号量
            mk_insert_node_to_ready_list(_tmp_task);
        }
    } else {
        _sem->value++;
    }

    mk_exit_critical(_status);

    return MK_SUCCESS;
}

/**
 * @brief 获取信号量
 * @param _sem
 * @return mk_code_t
 */
mk_code_t mk_sem_task(mk_sem_t* _sem, mk_size_t _tick_time) {
    mk_uint32_t _status;

    MK_ASSERT(_sem == MK_NULL, "_sem is null");

    _status = mk_enter_critical();
    if (_sem->value > 0) {
        _sem->value--;
        mk_exit_critical(_status);
        goto exit;
    }
    if (_tick_time < 0) {
        mk_exit_critical(_status);
        goto exit_time;
    } else {
        g_current_task->task_status = MK_TASK_STATUS_SUSPEND;

        // 插入信号量队列
        mk_insert_node_to_list_tail(&_sem->obj.list, g_current_task, GET_STR_DATA_ADDR_OFFSET(mk_task_t, sem_list));

        // 从就绪队列移除
        mk_delete_node_from_ready_list(g_current_task);

        mk_exit_critical(_status);

        // 进行任务调度
        mk_tack_scheduler();
    }

    return MK_BUSY;

exit:

    return MK_SUCCESS;

exit_time:
    return MK_TIMER_OUT;
}

/**
 * @brief 打印信号列表
 * @param _sem
 */
void mk_printf_sem_list(mk_sem_t* _sem) {
    mk_uint32_t _status;
    _status = mk_enter_critical();
    mk_task_t* tmp = _sem->obj.list.next;
    mkprintk("#########################################\r\n");
    mkprintk("#### print sem list\r\n");
    mkprintk("#### sem name:%s\r\n", _sem->obj.name);
    mkprintk("#### task all num:%d\r\n", _sem->obj.list.task_num);
    while (tmp != MK_NULL) {
        mkprintk("#### \ttask name:%s prio:%d delay:%d\r\n", tmp->task_name, tmp->prio, tmp->delay_systick);
        tmp = tmp->ready_list.next;
    }
    mkprintk("#########################################\r\n");
    mk_exit_critical(_status);
}

#endif
