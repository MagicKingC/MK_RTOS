#ifndef MKRTOS_CONFIG_H
#define MKRTOS_CONFIG_H

//ʱ���
#define USE_TIME_STAMP 0 //�ر�

//ʱ��Ƭ
#define USE_TIME_SLICE 1 //���� ������ͬһ�����ȼ�������ֻ�ܾ����б�ĵ�һ����������

//֧���������ȼ�
#define MK_PRIORITY_MAX	32

//�߳����ֳ���
#define MK_TASK_NAME_LEN 8

//ʱ���б���
#define MK_TICK_WHEEL_SIZE 16

//�Ƿ����ù�����
#define USE_SUSPEND_CTRL 1 //����

//main���������ȼ�
#define MAIN_TASK_PRIORITY MK_PRIORITY_MAX/2

#define MAIN_TASK_STACK_SIZE 512

#endif
