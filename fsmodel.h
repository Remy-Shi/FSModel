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
    FSMS_A = 0,
    FSMS_B,
    FSMS_C,
    FSMS_MAX,
};

typedef enum fsm_event{
    FSM_EVT_1 = 0,
    FSM_EVT_2,
    FSM_EVT_3,
    FSM_EVT_MAX,
}fsm_event_t;

typedef struct fsm_class *fsm_t;

typedef struct fsm_state_class
{
    fsm_err_t (*action_one)(fsm_t fsm_obj);
    fsm_err_t (*action_two)(fsm_t fsm_obj);
    fsm_err_t (*action_three)(fsm_t fsm_obj);
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
