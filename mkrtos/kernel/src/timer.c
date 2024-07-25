#include <mkdebug.h>
#include <mktimer.h>
#include <mktask.h>
#include <mkstr.h>

#ifdef MK_USE_SOFTTIMER

#define MK_INIT_TIMER_LIST_HEAD(_list, _node_type) _list = (_node_type){.prev = MK_NULL, .next = MK_NULL, .timer_num = 0};

static mk_list_timer_t soft_timer_list;

static mk_task_t timer_task;
static mk_stack_t timer_stack[MK_MAIN_STACK_SIZE];

/**
 * @brief 初始化定时器列表
 */
void mk_init_soft_timer_list(void) {
    MK_INIT_TIMER_LIST_HEAD(soft_timer_list, mk_list_timer_t);
}

/**
 * @brief 将数据插入定时器列表 使用尾插法
 * @param _node
 * @return mk_code_t
 */
mk_code_t mk_insert_node_to_soft_timer_list(mk_timer_t *_node) {
    if (_node == MK_NULL) {
        return MK_FAIL;
    }

    soft_timer_list.timer_num++;

    // 首节点
    if (soft_timer_list.next == MK_NULL && soft_timer_list.prev == MK_NULL) {
        soft_timer_list.next = _node;
        soft_timer_list.prev = _node;
        _node->prev = (mk_timer_t *)&soft_timer_list;
    } else {
        _node->prev = soft_timer_list.next;
        soft_timer_list.next->next = _node;
    }

    return MK_SUCCESS;
}

/**
 * @brief 更新定时器列表 : 取出已经到时间的定时器，并且调用回调，如果是重复运行的就会重新插入定时器列表
 */
void mk_updte_timer_list(void) {
    mk_timer_t *_tmp_timer;
    _tmp_timer = soft_timer_list.next;
    while (_tmp_timer != MK_NULL) {
        _tmp_timer->timerout_systick--;
        if (_tmp_timer->timerout_systick == 0) {
            _tmp_timer->callback_fun(_tmp_timer->para);
            if (_tmp_timer->flage == MK_TIMER_REPEAT) {
                _tmp_timer->timerout_systick = _tmp_timer->base_systick;
            } else {
                // 删除定时器节点
                _tmp_timer->next->prev = _tmp_timer->prev;
                _tmp_timer->prev->next = _tmp_timer->next;
                soft_timer_list.timer_num--;
            }
        }
        _tmp_timer = _tmp_timer->next;
    }
}

/**
 * @brief timer任务 ： 取出已经到时间的定时器
 * @param param
 */
void timer_entry(void *param) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("%s\r\n", __func__);
#endif
    while (MK_TRUE) {
        mk_updte_timer_list();
        // 判断列表是否存在定时器，如果没有则挂起任务
        if (soft_timer_list.timer_num == 0) {
            mk_task_suspend(g_current_task);
        } else {
            mk_task_tick_delay(1);
        }
    }
}

/**
 * @brief 创建定时器守护任务
 */
void mk_init_timer_task(void) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("entry %s\r\n", __func__);
#endif
    mk_task_init("timer", &timer_task, timer_entry, (void *)0x0, timer_stack, MK_SOFTTIMER_STACK_SIZE, MK_SOFTTIMER_TASK_PRIORITY, 10);
    mk_task_start(&timer_task);
}

/**
 * @brief 创建定时器
 * @param _name
 * @param _callback_fun
 * @param _para
 * @param _timer_systick
 * @param _flage
 * @return mk_timer_t
 */
mk_timer_t mk_timer_create(const char *_name, mk_timer_callback_fun_t _callback_fun,
                           void *_para, mk_size_t _timer_systick, mk_timer_flage_t _flage) {
    mk_timer_t __tmp_timer;

    mk_memset(__tmp_timer.name, 0, MK_SOFTTIMER_NAME_LEN);
    mk_strcpy(__tmp_timer.name, _name);

    __tmp_timer.base_systick = _timer_systick;
    __tmp_timer.timerout_systick = _timer_systick;
    __tmp_timer.para = _para;
    __tmp_timer.flage = _flage;
    __tmp_timer.status = MK_TIMER_INIT;
    __tmp_timer.callback_fun = _callback_fun;
    __tmp_timer.prev = MK_NULL;
    __tmp_timer.next = MK_NULL;

    return __tmp_timer;
}

/**
 * @brief 启动定时器
 * @param _timer
 * @return mk_code_t
 */
mk_code_t mk_start_timer(mk_timer_t *_timer) {
    if (_timer->status != MK_TIMER_RUNNING) {
        mk_insert_node_to_soft_timer_list(_timer);
        // 同时唤醒定时器守护任务
        if (timer_task.task_status == MK_TASK_STATUS_SUSPEND) {
            mk_task_resume(&timer_task);
        }
        return MK_SUCCESS;
    } else {
        return -MK_TIMER_ERR;
    }
}

/**
 * @brief 暂停定时
 * @param _timer
 * @return mk_code_t
 */
mk_code_t mk_stop_timer(mk_timer_t *_timer) {
    _timer->next->prev = _timer->prev;
    _timer->prev->next = _timer->next;
    _timer->status = MK_TIMER_STOP;
    soft_timer_list.timer_num--;
    return MK_SUCCESS;
}

#endif