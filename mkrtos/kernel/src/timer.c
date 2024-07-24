#include <mkdebug.h>
#include <mktimer.h>
#include <mktask.h>

#ifdef MK_USE_SOFTTIMER

#define MK_INIT_TIMER_LIST_HEAD(_list, _node_type) _list = (_node_type){.prev = MK_NULL, .next = MK_NULL};

static mk_timer_t _mk_timer_list;

static mk_task_t timer_task;
static mk_stack_t timer_stack[MK_MAIN_STACK_SIZE];

/**
 * @brief 初始化定时器列表
 */
void mk_init_soft_timer_list(void) {
    MK_INIT_TIMER_LIST_HEAD(_mk_timer_list, mk_timer_t);
}

/**
 * @brief timer任务 
 * 取出已经到时间的定时器
 * @param param
 */
void timer_entry(void* param) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("%s\r\n", __func__);
#endif
    while (MK_TRUE)
    {
       mk_task_suspend(g_current_task);
    }
    

}


/**
 * @brief 创建定时器守护任务
 */
void mk_init_timer_task(void) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("entry %s\r\n", __func__);
#endif
    mk_task_init("timer", &timer_task, timer_entry, (void*)0x0, timer_stack, MK_SOFTTIMER_STACK_SIZE, MK_SOFTTIMER_TASK_PRIORITY, 10);
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
mk_timer_t mk_timer_create(const char* _name,
                           mk_timer_callback_fun_t _callback_fun,
                           void* _para,
                           mk_size_t _timer_systick,mk_timer_flage_t _flage) {
    mk_timer_t __tmp_timer;

    __tmp_timer.base_systick = _timer_systick;
    __tmp_timer.para = _para;
    __tmp_timer.flage = _flage;
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
mk_code_t mk_start_timer(mk_timer_t* _timer) {
    mk_insert_node_to_soft_timer_list(_timer);
    return MK_SUCCESS;

}


/**
 * @brief 更新定时器列表
 * @return mk_code_t 
 */
mk_code_t mk_updte_timer_list(void) {
    return MK_SUCCESS;
}

/**
 * @brief 将数据插入定时器列表
 * @param _node
 * @return mk_code_t
 */
mk_code_t mk_insert_node_to_soft_timer_list(mk_timer_t* _node) {
    // if (_mk_timer_list.next == MK_NULL && )
    // {
    //     /* code */
    // }
    
    return MK_SUCCESS;
}

#endif