#include <mkrtos.h>
#include <mksemaphore.h>
#include <mktimer.h>

mk_sem_t sem;

mk_task_t task1;
mk_task_t task2;

static mk_stack_t task1_stk[1024];
static mk_stack_t task2_stk[1024];

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
    for (;;) {
        mk_sem_get(&sem);
        mkprintk("now task name: %s\r\n", mk_get_current_task_name());
    }
}

void test_time1(void* param) {
    mkprintk("run timer1 ********** %d \r\n",(int) param);
}

void test_time2(void* param) {
    mkprintk("run timer2 ********** \r\n");
}

int main(void) {
    mkprintk("entry main\r\n");
    mkprintk("now task name: %s\r\n", mk_get_current_task_name());

    mk_sem_create(&sem, "test_sem", 0);

    mk_timer_t tmp_timer1 = mk_timer_create("test_timer1", test_time1, (void *) 1, 100, MK_TIMER_REPEAT);
    mk_timer_t tmp_timer2 = mk_timer_create("test_timer2", test_time2, MK_NULL, 100, MK_TIMER_ONCE);
    mk_start_timer(&tmp_timer1);
    mk_start_timer(&tmp_timer2);

    mk_task_init("test1", &task1, task1_entry, (void*)0x0, task1_stk, 1024, 2, 10);
    mk_task_init("test2", &task2, task2_entry, (void*)0x0, task2_stk, 1024, 2, 10);

    mk_task_start(&task1);
    mk_task_start(&task2);

    while (1) {
        mkprintk("run main\r\n");
        mk_task_delay_ms(100);
    }
}
