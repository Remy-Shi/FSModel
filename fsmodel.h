/*
** fsmodel.h
** 2020-07-08
** Remy Shi
** 以事件驱动，整个状态机用事件去回调某个状态下的执行函数（动作或状态切换）。
*/

#ifndef FSMODEL_H
#define FSMODEL_H

#ifndef NULL
#define NULL (void *)0
#endif

typedef enum fsm_err{
    FSM_EOK = 0,
    FSM_ERROR,
    FSM_EINVALID,
}fsm_err_t;

enum fsm_state{
    FSM_STATE_A = 0,
    FSM_STATE_B,
    FSM_STATE_C,
    FSM_STATE_MAX,
};

typedef enum fsm_event{
    FSM_EVENT_1 = 0,
    FSM_EVENT_2,
    FSM_EVENT_3,
    FSM_EVENT_MAX,
}fsm_event_t;

typedef struct fsm_class *fsm_t;

typedef struct fsm_state_class
{
    fsm_err_t (*event_1_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_2_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_3_handler)(fsm_t fsm_obj);
    void *usr_data;
}*fsm_state_t;

typedef struct fsm_class
{
    fsm_state_t state_obj;
    void *usr_data;
}*fsm_t;

fsm_err_t fsm_state_set(fsm_t fsm_obj, fsm_state_t state_obj);
fsm_state_t fsm_state_get(fsm_t fsm_obj);
void fsm_example(void);

#endif
