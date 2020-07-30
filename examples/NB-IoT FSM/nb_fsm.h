/*
** nb_fsm.h
** 2020-07-30
** Remy Shi
** 以事件驱动，整个状态机用事件去回调某个状态下的执行函数（动作或状态切换）。
*/

#ifndef NB_FSM_H
#define NB_FSM_H

#ifndef NULL
#define NULL (void *)0
#endif

typedef enum fsm_err{
    FSM_EOK = 0,
    FSM_ERROR,
    FSM_EINVALID,
}fsm_err_t;

enum fsm_state{
    FSM_STATE_PWROFF = 0,
    FSM_STATE_DSLEEP,
    FSM_STATE_LSLEEP,
    FSM_STATE_OFFLINE, // NB standby but not online
    FSM_STATE_ONLINE,
    FSM_STATE_LOGGED,
    FSM_STATE_MAX,
};

typedef enum fsm_event{
    FSM_EVENT_PWROFF        = 0x00000001,
    FSM_EVENT_PWRON         = 0x00000002,
    FSM_EVENT_HW_AWAKEN     = 0x00000004,
    FSM_EVENT_SW_AWAKEN     = 0x00000008,
    FSM_EVENT_STANDBY_CHECK = 0x00000010,
    FSM_EVENT_NETWORK_CHECK = 0x00000020,
    FSM_EVENT_LOGIN_CHECK   = 0x00000040,
    FSM_EVENT_LOGIN         = 0x00000080,
    FSM_EVENT_LOGOUT        = 0x00000100,
}fsm_event_t;

typedef struct fsm_class *fsm_t;

typedef struct fsm_state_class
{
    fsm_err_t (*event_pwroff_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_pwron_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_hw_awaken_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_sw_awaken_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_standby_check_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_network_check_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_login_check_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_login_handler)(fsm_t fsm_obj);
    fsm_err_t (*event_logout_handler)(fsm_t fsm_obj);
    void *usr_data;
}*fsm_state_t;

typedef struct fsm_class
{
    fsm_state_t state_obj;
    void *usr_data;
}*fsm_t;

fsm_err_t fsm_state_set(fsm_t fsm_obj, fsm_state_t state_obj);
fsm_state_t fsm_state_get(fsm_t fsm_obj);

#endif /* The end of NB_FSM_H */
