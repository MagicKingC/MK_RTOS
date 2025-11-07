#include <mkrtos.h>
#include <mksemaphore.h>
#include <mkmutex.h>
#include <mktimer.h>

mk_sem_t sem;
mk_mutex_t mutex;

mk_task_t task1;
mk_task_t task2;
mk_task_t task3;
mk_task_t task4;
mk_task_t task5;

static mk_stack_t task1_stk[64];
static mk_stack_t task2_stk[64];
static mk_stack_t task3_stk[64];
static mk_stack_t task4_stk[64];
static mk_stack_t task5_stk[64];

int tmp_value = 0;

void task1_entry(void* param) {
    mkprintk("%s\r\n", __func__);
    for (;;) {
        mkprintk("now task name: %s\r\n", mk_get_current_task_name());
        mk_task_delay_ms(300);
        mk_sem_release(&sem);
    }
}

void task2_entry(void* param) {
    mkprintk("%s\r\n", __func__);
    for (;;)
    {
        mk_sem_task(&sem,MK_WAIT_FOREVERY);
        mkprintk("now task name: %s\r\n", mk_get_current_task_name());
        mk_now_task_delete();
    }
   
}

void task3_entry(void* param) {
    mkprintk("%s\r\n", __func__);
    for (;;)
    {
        mkprintk("now task name: %s\r\n", mk_get_current_task_name());
        mkprintk("priority: %d\r\n",mk_get_current_task_prio());
        mkprintk("task3 beginning\r\n");
        mk_mutex_lock(&mutex,MK_WAIT_FOREVERY);
        tmp_value++;
        mk_mutex_unlock(&mutex);
        mkprintk("priority: %d\r\n",mk_get_current_task_prio());
        mkprintk("task3 end\r\n");
        mk_task_delay_ms(300);
    }
   
}

void task4_entry(void* param) {
    mkprintk("%s\r\n", __func__);
    for (;;)
    {
        mkprintk("now task name: %s\r\n", mk_get_current_task_name());
        mkprintk("priority: %d\r\n",mk_get_current_task_prio());
        mkprintk("task4 beginning\r\n");
        mk_mutex_lock(&mutex,MK_WAIT_FOREVERY);
        tmp_value++;
        mk_mutex_unlock(&mutex);
        mkprintk("priority: %d\r\n",mk_get_current_task_prio());
        mkprintk("task4 end\r\n");
        mk_task_delay_ms(300);
    }
   
}

void task5_entry(void* param) {
    mkprintk("%s\r\n", __func__);
    for (;;)
    {
        mkprintk("now task name: %s\r\n", mk_get_current_task_name());
        mkprintk("priority: %d\r\n",mk_get_current_task_prio());
        mkprintk("task5 beginning\r\n");
        mk_mutex_lock(&mutex,MK_WAIT_FOREVERY);
        mkprintk("task5 mutex\r\n");
        mk_task_delay_ms(1000);
        tmp_value++;
        mk_mutex_unlock(&mutex);
        mkprintk("priority: %d\r\n",mk_get_current_task_prio());
        mkprintk("task5 end\r\n");
        mk_task_delay_ms(300);
    }
   
}

void test_time1(void* param) {
    mkprintk("run timer1 ********** %d \r\n",(int) param);
}

void test_time2(void* param) {
    mkprintk("run timer2 ********** \r\n");
}

/**
 * @brief 主函数
 * @return int 
 */
int main(void) {
    const char* test = "test";
    mkprintk("entry main\r\n");
    mkprintk("now task name: %s\r\n", mk_get_current_task_name());

    mk_sem_create(&sem, "test_sem", 0);
    mk_mutex_create(&mutex, "test_mutex");

    mk_timer_t tmp_timer1 = mk_timer_create("test_timer1", test_time1, (void *) 4, 100, MK_TIMER_REPEAT);
    mk_timer_t tmp_timer2 = mk_timer_create("test_timer2", test_time2, MK_NULL, 100, MK_TIMER_ONCE);

    mk_start_timer(&tmp_timer1);
    mk_start_timer(&tmp_timer2);

    mk_task_init("test1", &task1, task1_entry, (void*)test, task1_stk, 1024, 2, 10);
    mk_task_init("test2", &task2, task2_entry, (void*)0x0, task2_stk, 1024, 2, 10);
    mk_task_init("test3", &task3, task3_entry, (void*)0x0, task3_stk, 1024, 3, 10);
    mk_task_init("test4", &task4, task4_entry, (void*)0x0, task4_stk, 1024, 4, 10);
    mk_task_init("test5", &task5, task5_entry, (void*)0x0, task5_stk, 1024, 5, 10);

    mk_task_start(&task1);
    mk_task_start(&task2);
    mk_task_start(&task3);
    mk_task_start(&task4);
    mk_task_start(&task5);

    while (1) {
        // mkprintk("run main\r\n");
        mkprintk("now task name: %s\r\n", mk_get_current_task_name());
        mkprintk("priority: %d\r\n",mk_get_current_task_prio());
        mk_task_delay_ms(100);
    }
}
