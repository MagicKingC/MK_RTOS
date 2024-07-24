#include <mktask.h>

#ifdef MK_USING_SYS_DEBUG
#include <mkdebug.h>
#endif

mk_task_t idle_task;
mk_stack_t idle_stack[MK_IDLE_STACK_SIZE];
/**
 * @brief 空闲任务
 * @param param
 */
void idle_entry(void* param) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("entry %s\r\n", __func__);
#endif
    while (MK_TRUE) {

    }
}

/**
 * @brief 初始化空闲任务
 */
void mk_init_idle_task(void) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("entry %s\r\n", __func__);
#endif

    mk_task_init("idle", &idle_task, idle_entry, (void*)0x0, idle_stack, MK_IDLE_STACK_SIZE, MK_IDLE_TASK_PRIORITY,10);

    mk_task_start(&idle_task);
}