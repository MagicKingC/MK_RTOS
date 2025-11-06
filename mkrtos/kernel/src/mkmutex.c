#include <mkdebug.h>
#include <mklist.h>
#include <mkmutex.h>
#include <mkstr.h>
#include <mktask.h>

/**
 * @brief 创建互斥锁
 * @param _mutex 互斥锁指针
 * @param _name 互斥锁名称
 * @return MK_SUCCESS 成功
 */
mk_code_t mk_mutex_create(mk_mutex_t *_mutex, const char *_name)
{
    _mutex->is_use = MK_TRUE;
    _mutex->owner = MK_NULL;
    _mutex->owner_prio = -1;
    _mutex->status = MK_MUTEX_UNLOCK;

#if MK_USE_ASSERT
    MK_ASSERT(_mutex == MK_NULL, "_mutex(mk_mutex_create) is null");
#else
    return MK_FAIL;
#endif

    mk_ipc_init(&_mutex->obj, _name);

    return MK_SUCCESS;
}

/**
 * @brief 获取互斥锁
 * @param _mutex 互斥锁指针
 * 
 */
mk_code_t mk_mutex_lock(mk_mutex_t *_mutex, mk_ticks_t _tick_time)
{
    mk_uint32_t _status;
    mk_size_t _current_prio;

#if MK_USE_ASSERT
    MK_ASSERT(_mutex == MK_NULL, "_mutex(mk_mutex_lock) is null");
#else
    return MK_FAIL;
#endif

    _status = mk_enter_critical();

    if (_mutex->status == MK_MUTEX_UNLOCK)
    {
        _mutex->owner = g_current_task; // 设置锁拥有者
        _mutex->owner_prio = g_current_task->prio; // 锁拥有者优先级
        _mutex->owner->mutex_prio = g_current_task->prio;
        _mutex->status = MK_MUTEX_LOCK;
        mk_exit_critical(_status);
        goto exit;
    }

    // 判断当前任务的优先级是否大于锁拥有者的优先级，如果是就提高锁拥有者的优先级
    if (g_current_task->prio > _mutex->owner_prio)
    {
        _mutex->owner->mutex_prio = _mutex->owner_prio;
        _mutex->owner->prio = g_current_task->prio;
        if (_mutex->owner->task_status == MK_TASK_STATUS_READY)
        {
            mk_delete_node_from_ready_list(_mutex->owner);
            mk_insert_node_to_ready_list(_mutex->owner);
        }
    }

    // 判断是否为永久阻塞
    if (_tick_time != MK_WAIT_FOREVERY) {
        // 插入延迟队列
        g_current_task->delay_systick = _tick_time;
        mk_insert_node_to_delay_list(g_current_task);
    } else {
        // 插入互斥量列表
        mk_insert_node_to_list_tail(&_mutex->obj.list, g_current_task, GET_STR_DATA_ADDR_OFFSET(mk_task_t, mutex_list));
    }
    // 从就绪队列移除
    mk_delete_node_from_ready_list(g_current_task);

    // 设置任务状态为阻塞
    g_current_task->task_status = MK_TASK_STATUS_BLOCK;

    mk_exit_critical(_status);

    // 进行任务调度
    mk_tack_scheduler();

exit:
    return MK_SUCCESS;
}

/**
 * @brief 释放互斥锁
 * @param _mutex 互斥锁指针
 */
mk_code_t mk_mutex_unlock(mk_mutex_t *_mutex)
{
    mk_task_t* _tmp_task = MK_NULL;
    mk_uint32_t _status;

#if MK_USE_ASSERT
    MK_ASSERT(_mutex == MK_NULL, "_mutex(mk_mutex_unlock) is null");
#else
    return MK_FAIL;
#endif

    _status = mk_enter_critical();

    if (_mutex->status == MK_MUTEX_LOCK)
    {
        // 判断是否是锁拥有者
        if (_mutex->owner != g_current_task) {
            MK_ASSERT(MK_TRUE, "_mutex(mk_mutex_unlock) is not owner");
        }else{
            // 恢复锁拥有者的优先级
           _mutex->owner->prio = _mutex->owner->mutex_prio;
           _mutex->owner->mutex_prio = -1;
        }

        // 获取头部节点
        _tmp_task = mk_get_node_from_list(&_mutex->obj.list);
        if (_tmp_task != MK_NULL) {
            // 将任务从互斥锁列表移除
            mk_delete_node_from_list(&_mutex->obj.list, _tmp_task, GET_STR_DATA_ADDR_OFFSET(mk_task_t, mutex_list));
            mk_insert_node_to_ready_list(_tmp_task);
        }
        _mutex->status = MK_MUTEX_UNLOCK;
    }
    
    mk_exit_critical(_status);
    return MK_SUCCESS;
}


/**
 * @brief 获取互斥锁列表
 */
void mk_printf_mutex_list(mk_mutex_t *_mutex)
{
    mk_uint32_t _status;
    _status = mk_enter_critical();
    mk_task_t* tmp = _mutex->obj.list.next;
    mkprintk("#########################################\r\n");
    mkprintk("#### print mutex list\r\n");
    mkprintk("#### mutex name:%s\r\n", _mutex->obj.name);
    mkprintk("#### task all num:%d\r\n", _mutex->obj.list.task_num);
    while (tmp != MK_NULL) {
        mkprintk("#### \ttask name:%s prio:%d delay:%d\r\n", tmp->task_name, tmp->prio, tmp->delay_systick);
        tmp = tmp->mutex_list.next;
    }
    mkprintk("#########################################\r\n");
    mk_exit_critical(_status);
}