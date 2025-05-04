#include <mkdebug.h>
#include <mklist.h>
#include <mktask.h>

#define MK_PRIORITY_INDEX_BUFF_SIZE (MK_TASK_PRIORITY_NUM / 32 + ((MK_TASK_PRIORITY_NUM % 32 == 0) ? 0 : 1))
static mk_uint32_t _mk_priority_index_buff[MK_PRIORITY_INDEX_BUFF_SIZE];

#define MK_INIT_LIST_HEAD(_list, _node_type) _list = (_node_type){.prev = MK_NULL, .next = MK_NULL, .task_num = 0};

typedef enum _LIST_TYPE {
    MK_READY_LIST = 0,
    MK_DELAY_LIST,
} mk_list_type_t;

/**
 * @brief 就绪列表
 */
static mk_ready_list_t _mk_task_ready_list[MK_TASK_PRIORITY_NUM];
/**
 * @brief 延迟列表
 */
static mk_delay_list_t _mk_task_delay_list;
/**
 * @brief 软件定时器列表
 */

/**
 * @brief 用于查找 1 在当前数字的第几位
 */
const mk_uint8_t _bitmap[] = {
    /* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};

/**
 * @brief 初始化优先级位表
 */
void mk_init_task_bit_map(void) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("entry %s\r\n", __func__);
    mkprintk("MK_PRIORITY_INDEX_BUFF_SIZE %ld\r\n", MK_PRIORITY_INDEX_BUFF_SIZE);
#endif
    MK_ASSERT((MK_TASK_PRIORITY_NUM < 32), "task priority num must bigger 32");
    for (mk_base_t i = 0; i < MK_PRIORITY_INDEX_BUFF_SIZE; i++) {
        _mk_priority_index_buff[i] = 0;
    }
}

/**
 * @brief 设置优先级位表
 * @param _index
 */
void mk_set_bitmap(mk_base_t _index) {
    mk_uint32_t _buff_index = _index / 32;
    mk_uint32_t _buff_bit = _index % 32;
    _mk_priority_index_buff[_buff_index] |= 0x01 << _buff_bit;
}

/**
 * @brief 清除优先级位表
 * @param _index
 */
void mk_clear_bitmap(mk_base_t _index) {
    mk_uint32_t _buff_index = _index / 32;
    mk_uint32_t _buff_bit = _index % 32;
    _mk_priority_index_buff[_buff_index] &= ~(0x01 << _buff_bit);
}

/**
 * @brief 查找最高优先级任务
 * @return mk_base_t
 */
mk_base_t mk_find_hight_priority(void) {
    mk_base_t _index = 0;
    mk_uint16_t low16bit;
    mk_uint16_t hight16bit;
    mk_uint8_t low8bit;
    mk_uint8_t hight8bit;

    for (mk_base_t i = 0; i < MK_PRIORITY_INDEX_BUFF_SIZE; i++) {
        if (_mk_priority_index_buff[i] != 0) {
            low16bit = _mk_priority_index_buff[i] & 0x0FFFF;
            if (low16bit != 0) {
                low8bit = low16bit & 0x0FF;
                if (low8bit != 0) {
                    _index = _bitmap[low8bit] + i * 32;
                    break;
                }
                hight8bit = (low16bit >> 8) & 0x0FF;
                _index = _bitmap[hight8bit] + 8 + i * 32;
                break;
            }

            hight16bit = (_mk_priority_index_buff[i] >> 16) & 0x0FFFF;
            low8bit = hight16bit & 0x0FF;
            if (low8bit != 0) {
                _index = _bitmap[low8bit] + 16 + i * 32;
                break;
            }
            hight8bit = (hight16bit >> 8) & 0x0FF;
            _index = _bitmap[hight8bit] + 24 + i * 32;
            break;
        }
    }
    return _index;
}

/**
 * @brief 列表初始化(就绪列表 延迟队列)
 * @param _head
 */
void mk_init_system_list(void) {
    for (mk_base_t i = 0; i < MK_TASK_PRIORITY_NUM; i++) {
        MK_INIT_LIST_HEAD(_mk_task_ready_list[i], mk_ready_list_t);
    }
    MK_INIT_LIST_HEAD(_mk_task_delay_list, mk_delay_list_t);
}

/**
 * @brief 列表初始化
 * @param _list
 */
void mk_init_list(mk_list_t *_list) {
    MK_INIT_LIST_HEAD(*_list, mk_list_t);
}

/**
 * @brief 尾插法
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_insert_node_to_list_tail(mk_list_t *_list, mk_task_t *_task, mk_size_t _offset) {
    _list->task_num++;

    mk_list_t *_tmp_task_list = (mk_list_t *)((mk_size_t)_task + _offset);

    // 第一次插入
    if (_list->prev == MK_NULL && _list->next == MK_NULL) {
        _list->prev = _task;
        _list->next = _task;

        _tmp_task_list->prev = MK_NULL;
        _tmp_task_list->next = MK_NULL;

    } else {
        _tmp_task_list->prev = _list->prev;
        _tmp_task_list->next = MK_NULL;

        ((mk_list_t *)(((mk_size_t)_list->prev) + _offset))->next = _task;

        _list->prev = _task;
    }

    return MK_SUCCESS;
}

/**
 * @brief 取出列表收个节点
 * @param _list
 * @return mk_task_t*
 */
mk_task_t *mk_get_node_from_list(mk_list_t *_list) {
    return _list->next;
}

/**
 * @brief 将任务从队列中删除
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_delete_node_from_list(mk_list_t *_list, mk_task_t *_task, mk_size_t _offset) {
    mk_list_t *_tmp_task_list;

    if (_list->task_num > 0) {
        _list->task_num--;
    } else {
        return MK_FAIL;
    }

    _tmp_task_list = (mk_list_t *)((mk_size_t)_task + _offset);

    if (_list->task_num == 0) // 只有一个节点
    {
        _list->prev = MK_NULL;
        _list->next = MK_NULL;

        if (_offset == GET_STR_DATA_ADDR_OFFSET(mk_task_t, ready_list)) {
            mk_clear_bitmap(_task->prio);
        }
    } else if (_tmp_task_list->next == MK_NULL) { // 尾节点
        _list->prev = _tmp_task_list->prev;
        ((mk_list_t *)((mk_size_t)(_tmp_task_list->prev) + _offset))->next = MK_NULL;
    } else if (_tmp_task_list->prev == MK_NULL) { // 头节点
        _list->next = _tmp_task_list->next;
        ((mk_list_t *)((mk_size_t)(_tmp_task_list->next) + _offset))->prev = MK_NULL;

    } else { // 非首尾节点
        ((mk_list_t *)((mk_size_t)(_tmp_task_list->prev) + _offset))->next = _tmp_task_list->next;
        ((mk_list_t *)((mk_size_t)(_tmp_task_list->next) + _offset))->prev = _tmp_task_list->prev;
    }

    _tmp_task_list->prev = MK_NULL;
    _tmp_task_list->next = MK_NULL;

    return MK_SUCCESS;
}

/**
 * @brief 将任务由头部移动到尾部
 * @param _task
 * @return mk_code_t
 */
static mk_code_t mk_move_node_to_tail(mk_list_t *_list, mk_task_t *_task) {
    mk_size_t _prio = 0;

    if (_task->prio > MK_TASK_PRIORITY_NUM) {
        return MK_FAIL;
    }
    _prio = _task->prio;

    if (_list[_prio].task_num <= 1) {
        goto fun_end;
    }

    _list[_prio].next->ready_list.next->ready_list.prev = MK_NULL;
    _list[_prio].next = _list[_prio].next->ready_list.next;

    _list[_prio].prev->ready_list.next = _task;

    _task->ready_list.prev = _list[_prio].prev;
    _task->ready_list.next = MK_NULL;

    _list[_prio].prev = _task;

fun_end:
    return MK_SUCCESS;
}

/**
 * @brief 就绪列表 将任务插入就绪列表
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_insert_node_to_ready_list(mk_task_t *_task) {
    if (_task->prio > MK_TASK_PRIORITY_NUM) {
        return MK_FAIL;
    }
    mk_set_bitmap(_task->prio);
    _task->task_status = MK_TASK_STATUS_READY;
    mk_insert_node_to_list_tail(&_mk_task_ready_list[_task->prio], _task,
                                GET_STR_DATA_ADDR_OFFSET(mk_task_t, ready_list));
    return MK_SUCCESS;
}

/**
 * @brief 就绪列表 将数据移动队尾
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_move_node_to_ready_list_tail(mk_task_t *_task) {
    return mk_move_node_to_tail(_mk_task_ready_list, _task);
}

/**
 * @brief 将任务从就绪列表删除
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_delete_node_from_ready_list(mk_task_t *_task) {
    if (_task->prio > MK_TASK_PRIORITY_NUM) {
        mkprintk("\r\nerr: task pro:%d\r\n", _task->prio);
        return MK_FAIL;
    }
    return mk_delete_node_from_list(&_mk_task_ready_list[_task->prio], _task,
                                    GET_STR_DATA_ADDR_OFFSET(mk_task_t, ready_list));
}

/**
 * @brief 打印就绪列表
 * @param _prio
 */
void mk_printf_ready_list(mk_size_t _prio) {
    mk_uint32_t _status;

    if (_prio > MK_TASK_PRIORITY_NUM) {
        mkprintk("\r\nerr: task pro:%d\r\n", _prio);
        return;
    }
    _status = mk_enter_critical();
    mk_task_t *tmp = _mk_task_ready_list[_prio].next;

    mkprintk("#########################################\r\n");
    mkprintk("#### print ready list\r\n");
    mkprintk("#### task prio:%d\r\n", _prio);
    mkprintk("#### task all num:%d\r\n", _mk_task_ready_list[_prio].task_num);
    while (tmp != MK_NULL) {
        mkprintk("#### \ttask name:%s prio:%d delay:%d\r\n", tmp->task_name, tmp->prio, tmp->delay_systick);
        tmp = tmp->ready_list.next;
    }
    mkprintk("#########################################\r\n");
    mk_exit_critical(_status);
}

/**
 * @brief 延迟列表
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_insert_node_to_delay_list(mk_task_t *_task) {
    _task->task_status = MK_TASK_STATUS_SUSPEND;
    return mk_insert_node_to_list_tail(&_mk_task_delay_list, _task, GET_STR_DATA_ADDR_OFFSET(mk_task_t, delay_list));
}

/**
 * @brief 将任务从延迟列表删除
 * @param _task
 * @return mk_code_t
 */
mk_code_t mk_delete_node_from_delay_list(mk_task_t *_task) {
    return mk_delete_node_from_list(&_mk_task_delay_list, _task, GET_STR_DATA_ADDR_OFFSET(mk_task_t, delay_list));
}

/**
 * @brief 更新延迟列表
 */
/**
 * 该函数需要重构。重构思想：将定时器重小到大排序，然后每次只判断第一个节点时间是否已经到了，如果到了就将该任务变为
 * 就绪态，同时将下一个节点的数据减去当前节点的延时时间
 */
void mk_update_delay_list(void) {
    mk_task_t *_task = MK_NULL;
    mk_task_t *_task_tmp = MK_NULL;
    mk_size_t _task_num = _mk_task_delay_list.task_num;
    if (_task_num == 0) {
        return;
    }

    _task = _mk_task_delay_list.next;
    for (mk_size_t i = 0; i < _task_num; i++) {
        if (_task->delay_systick <= 0) {
            _task_tmp = _task;
            _task = _task->delay_list.next;
            _task_tmp->delay_systick = 0;
            mk_delete_node_from_delay_list(_task_tmp);
            mk_insert_node_to_ready_list(_task_tmp);
            continue;
        } else {
            _task->delay_systick--;
        }
        _task = _task->delay_list.next;
    }
}

/**
 * @brief 打印延迟列表
 */
void mk_printf_delay_list(void) {
    mk_uint32_t _status;

    _status = mk_enter_critical();
    mk_task_t *tmp = _mk_task_delay_list.next;

    mkprintk("#########################################\r\n");
    mkprintk("#### print delay list\r\n");
    mkprintk("#### task num:%d\r\n", _mk_task_delay_list.task_num);
    if (_mk_task_delay_list.task_num <= 0) {
        mkprintk("#########################################\r\n");
        mk_exit_critical(_status);
        return;
    }
    while (tmp != MK_NULL) {
        mkprintk("#### \ttask name:%s\r\n", tmp->task_name);
        tmp = tmp->delay_list.next;
    }
    mkprintk("#########################################\r\n");
    mk_exit_critical(_status);
}

/**
 * @brief 取出优先级最高的任务,同时将队列移动到队尾
 * @param _highest_prio_index
 * @return mk_task_t*
 */
mk_task_t *mk_get_node_from_ready_list(mk_size_t _highest_prio_index) {
    mk_task_t *_task = MK_NULL;

    if (_mk_task_ready_list[_highest_prio_index].task_num > 0) {
        _task = _mk_task_ready_list[_highest_prio_index].next;
        mk_move_node_to_ready_list_tail(_task);
    }

    return _task;
}