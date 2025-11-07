#include <mkinc.h>
#include <mklist.h>
#include <mkstr.h>
#include <mktask.h>

#ifdef MK_USING_SYS_DEBUG
#include <mkdebug.h>
#endif

mk_task_t *g_current_task;
mk_stack_t *g_current_task_sp;
mk_stack_t *g_next_task_sp;

typedef struct _mk_statck_manual_ops_reg {
    mk_uint32_t r4;
    mk_uint32_t r5;
    mk_uint32_t r6;
    mk_uint32_t r7;
    mk_uint32_t r8;
    mk_uint32_t r9;
    mk_uint32_t r10;
    mk_uint32_t r11;
} mk_statck_manual_ops_reg_t;

typedef struct _mk_statck_auto_ops_reg {
    mk_uint32_t r0;
    mk_uint32_t r1;
    mk_uint32_t r2;
    mk_uint32_t r3;
    mk_uint32_t r12;
    mk_uint32_t lr;
    mk_uint32_t pc;
    mk_uint32_t xpsp;
} mk_statck_auto_ops_reg_t;

typedef struct _mk_statck_ops {
    mk_statck_manual_ops_reg_t statck_manual_ops;
    mk_statck_auto_ops_reg_t statck_auto_ops;
} mk_stack_ops_reg_t;

/**
 * @brief 初始化任务栈
 * @param _func_entry
 * @param _param
 * @param _stack
 * @param _stack_size
 * @return mk_stack_t*
 */
mk_stack_t *_mk_init_stack(void (*_func_entry)(void *), void *_param, mk_stack_t *_stack, mk_size_t _stack_size) {
    mk_stack_ops_reg_t *stack_ops_reg;

    mk_stack_t *_stack_tmp;

#if MK_STACK_DIRECTION
    _stack_tmp = (mk_stack_t *)MK_ALIGN_DOWN((mk_uint32_t)(_stack), 8);
    _stack_tmp = _stack_tmp + sizeof(mk_stack_ops_reg_t);
#else
    _stack_tmp = (mk_stack_t *)MK_ALIGN_DOWN((mk_uint32_t)(_stack + _stack_size), 8);
    _stack_tmp = _stack_tmp - sizeof(mk_stack_ops_reg_t);

#endif
    stack_ops_reg = (mk_stack_ops_reg_t *)(_stack_tmp);

    stack_ops_reg->statck_manual_ops.r4 = 0x04;
    stack_ops_reg->statck_manual_ops.r5 = 0x05;
    stack_ops_reg->statck_manual_ops.r6 = 0x06;
    stack_ops_reg->statck_manual_ops.r7 = 0x07;
    stack_ops_reg->statck_manual_ops.r8 = 0x08;
    stack_ops_reg->statck_manual_ops.r9 = 0x09;
    stack_ops_reg->statck_manual_ops.r10 = 0x10;
    stack_ops_reg->statck_manual_ops.r11 = 0x11;

    stack_ops_reg->statck_auto_ops.xpsp = (mk_uint32_t)(1 << 24);
    stack_ops_reg->statck_auto_ops.pc = (mk_uint32_t)_func_entry;
    stack_ops_reg->statck_auto_ops.lr = 0x14;
    stack_ops_reg->statck_auto_ops.r12 = 0x12;
    stack_ops_reg->statck_auto_ops.r3 = 0x03;
    stack_ops_reg->statck_auto_ops.r2 = 0x02;
    stack_ops_reg->statck_auto_ops.r1 = 0x01;
    stack_ops_reg->statck_auto_ops.r0 = (mk_uint32_t)_param;

    return _stack_tmp;
}

/**
 * @brief 初始化任务
 * @param _task_name 任务名
 * @param _task 任务指针
 * @param _entry 任务入口
 * @param _param 参数
 * @param _stack 堆栈
 * @param _stack_size 堆栈大小
 * @param _task_pro 优先级
 * @param _timer_tick 时间片
 * @return mk_code_t
 */
mk_code_t mk_task_init(const char *_task_name, mk_task_t *_task, void (*_func_entry)(void *), void *_param,
                       mk_stack_t *_stack, mk_size_t _stack_size, mk_size_t _task_pro, mk_size_t _timer_tick) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("%s(%s)\r\n", __func__, _task_name);
#endif

    _task->task_sp = _mk_init_stack(_func_entry, _param, _stack, _stack_size);

    _task->prio = _task_pro;
    _task->mutex_prio = _task_pro;
    _task->delay_systick = 0;

    _task->timer_tick = _timer_tick;
    _task->base_timer_tick = _timer_tick;

    _task->ready_list.prev = MK_NULL;
    _task->ready_list.next = MK_NULL;

    _task->delay_list.prev = MK_NULL;
    _task->delay_list.next = MK_NULL;

    _task->task_status = MK_TASK_STATUS_CTREATE;

    mk_memset(_task->task_name, 0, MK_TASK_NAME_LEN);
    mk_strcpy(_task->task_name, _task_name);

    return MK_SUCCESS;
}

/**
 * @brief 将任务插入就绪队列
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_task_start(mk_task_t *_task) {
    mk_code_t res = MK_FAIL;
    mk_uint32_t _status = mk_enter_critical();
     
    if (_task->task_status == MK_TASK_STATUS_CTREATE) {
        res = mk_insert_node_to_ready_list(_task);
    }
    mk_exit_critical(_status);
    return res;
}

/**
 * @brief 任务状态转换
 * @param _task
 * @param _status
 * @return mk_code_t
 */
mk_code_t mk_task_change_status(mk_task_t *_task,mk_task_status_t _task_status) {
    mk_code_t res = MK_FAIL;
    mk_uint32_t _status = mk_enter_critical();
    if (_task->task_status == MK_TASK_STATUS_RUNNING || _task->task_status == MK_TASK_STATUS_READY) {
        _task->task_status = _task_status;
        res = mk_delete_node_from_ready_list(_task);
    }
    mk_exit_critical(_status);
    if (res == MK_SUCCESS) {
        mk_tack_scheduler();
    }
    return res;
}

/**
 * @brief 任务挂起
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_task_suspend(mk_task_t *_task) {
    return mk_task_change_status(_task,MK_TASK_STATUS_SUSPEND);
}

/**
 * @brief 任务阻塞
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_task_block(mk_task_t *_task) {
    return mk_task_change_status(_task,MK_TASK_STATUS_BLOCK);
}

/**
 * @brief 任务唤醒
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_task_resume(mk_task_t *_task) {
    mk_code_t res = MK_FAIL;
    mk_uint32_t _status = mk_enter_critical();
    if (_task->task_status == MK_TASK_STATUS_SUSPEND || _task->task_status == MK_TASK_STATUS_BLOCK) {
        _task->task_status = MK_TASK_STATUS_READY;
        res = mk_insert_node_to_ready_list(_task);
    }
    mk_exit_critical(_status);
    return res;
}

/**
 * @brief 放回当前任务指针
 * @return mk_task_t*
 */
mk_task_t *mk_get_current_task(void) {
    return g_current_task;
}

/**
 * @brief 获取当前任务优先级
 * @return mk_size_t
 */
mk_size_t mk_get_current_task_prio(void) {
    return g_current_task->prio;
}

/**
 * @brief 获取当前任务名字
 * @return mk_size_t
 */
char *mk_get_current_task_name(void) {
    return g_current_task->task_name;
}

/**
 * @brief 移除任务
 * @param _task 任务指针
 */
void mk_task_delete(mk_task_t *_task) {
    _task->task_status = MK_TASK_STATUS_COLOSE;
    mk_delete_node_from_ready_list(_task);
    mk_tack_scheduler();
}

/**
 * @brief 删除当前任务
 */
void mk_now_task_delete(void){
    g_current_task->task_status = MK_TASK_STATUS_COLOSE;
    mk_delete_node_from_ready_list(g_current_task);
    mk_tack_scheduler();
}

/**
 * @brief 延迟函数
 * @param _systick_times 延迟滴答数
 * @return mk_code_t 系统码
 */
mk_code_t mk_task_tick_delay(mk_size_t _systick_times) {
    mk_uint32_t _status = mk_enter_critical();

    g_current_task->delay_systick = _systick_times;

    // 插入休眠队列
    mk_insert_node_to_delay_list(g_current_task);

    // 从就绪队列移除
    mk_delete_node_from_ready_list(g_current_task);

    mk_exit_critical(_status);

    // 进行任务调度
    mk_tack_scheduler();

    return MK_SUCCESS;
}

/**
 * @brief 延迟函数
 * @param _ms
 */
void mk_task_delay_ms(mk_size_t _ms) {
    mk_task_tick_delay(_ms);
}

/**
 * @brief 进行任务调度
 */
void mk_tack_scheduler() {
    mk_size_t priority_num;
    mk_task_t *hight_task;

    mk_uint32_t _status = mk_enter_critical();
    // 更新延迟队列
    mk_update_delay_list();

    // 查找最高优先级任务 prio越小，优先级越高
    priority_num = mk_find_hight_priority();
    if (priority_num < g_current_task->prio) {
        goto sw_task;
    }

    // 判断时间片
    if (g_current_task->task_status == MK_TASK_STATUS_RUNNING) {
        if (g_current_task->timer_tick > 0) {
            g_current_task->timer_tick--;
            goto _exit;
        }
        g_current_task->timer_tick = g_current_task->base_timer_tick;
    }

sw_task:
    hight_task = mk_get_node_from_ready_list(priority_num);

    if (hight_task == MK_NULL) {
        goto _exit;
    }

    g_current_task = hight_task;
    g_current_task->task_status = MK_TASK_STATUS_RUNNING;
    g_next_task_sp = &hight_task->task_sp;

    mk_exit_critical(_status);

    mk_trigger_pendsv();

    return;

_exit:
    mk_exit_critical(_status);
}