#include <mkinc.h>
#include <mklist.h>
#include <mkrtos.h>
#include <mktask.h>
#include <mktimer.h>
#ifdef MK_USING_SYS_DEBUG
#include <mkdebug.h>
#endif

MK_RTOS_EXT int main(void);
static mk_task_t main_task;
static mk_stack_t main_stack[MK_MAIN_STACK_SIZE];

/**
 * @brief main任务
 * @param param
 */
void main_entry(void* param) {
    mk_start_systick();
#ifdef MK_USING_SYS_DEBUG
    mkprintk("%s\r\n", __func__);
#endif
    main();
}

/**
 * @brief 初始化main 任务
 */
static void mk_init_main_task(void) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("entry %s\r\n", __func__);
#endif

    mk_task_init("main", &main_task, main_entry, (void*)0x0, main_stack, MK_MAIN_STACK_SIZE, MK_MAIN_TASK_PRIORITY, 10);
    mk_task_start(&main_task);
}

/**
 * @brief 启动kernel
 * 1、初始化板载硬件
 * 2、初始化systick
 * 3、初始化定时器列表
 * 4、初始化就绪列表
 * 5、初始化时间片列表
 * 6、初始化优先级列表
 * 7、创建空闲任务
 * 8、创建main任务
 * 9、触发svc中断进行任务切换
 */
static void mk_startup_kernel(void) {
    mk_size_t priority_num = 0;
    mk_task_t* hight_task;
#ifdef MK_USING_SYS_DEBUG
    mkprintk("entry %s\r\n", __func__);
#endif

    // 初始化位图
    mk_init_task_bit_map();
    // 初始化列表
    mk_init_system_list();
#ifdef MK_USE_SOFTTIMER
    // 初始化软定时器列表
    mk_init_soft_timer_list();
#endif
    // 初始化空闲任务
    mk_init_idle_task();
    // 初始化main任务
    mk_init_main_task();
#ifdef MK_USE_SOFTTIMER
    // 初始化定时器任务
    mk_init_timer_task();
#endif
    // 初始化时钟
    mk_init_systick();

    // 查找最高优先级任务，并且设置调用
    g_current_task = MK_NULL;
    priority_num = mk_find_hight_priority();
    hight_task = mk_get_node_from_ready_list(priority_num);
    g_current_task = hight_task;
    g_current_task->task_status = MK_TASK_STATUS_RUNNING;
    g_current_task_sp = (mk_stack_t*)&g_current_task->task_sp;

#ifdef MK_USING_SYS_DEBUG
    mkprintk("run kernel\r\n");
#endif
    // 触发svc中断进行任务切换
    mk_call_svc();
}

/**
 * @brief kernel 入口
 * @return int
 */
int _mk_rtos_sys_main(void) {
    mk_init_log();
#ifdef MK_USING_SYS_DEBUG
    mkprintk("entry %s\r\n", __func__);
#endif
    mk_startup_kernel();
    return 0;
}