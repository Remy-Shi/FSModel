/*
** nb_fsm.c
** 2020-07-30
** Remy Shi
** 以事件驱动，整个状态机用事件去回调某个状态下的执行函数（动作或状态切换）。
*/

#include "nb_fsm.h"
#include <stdio.h>

fsm_err_t fsm_state_set(fsm_t fsm_obj, fsm_state_t state_obj)
{
    if((NULL == fsm_obj) || (NULL == state_obj)){
        return FSM_EINVALID;
    }
    fsm_obj->state_obj = state_obj;
    return FSM_EOK;
}


fsm_state_t fsm_state_get(fsm_t fsm_obj)
{
    if(NULL == fsm_obj){
        return NULL;
    }
    return fsm_obj->state_obj;
}


static fsm_err_t ignore_event(fsm_t fsm_obj);
static fsm_err_t nb_power_on(fsm_t fsm_obj);
static fsm_err_t nb_power_off(fsm_t fsm_obj);
static fsm_err_t nb_hw_awaken(fsm_t fsm_obj);
static fsm_err_t nb_sw_awaken(fsm_t fsm_obj);
static fsm_err_t nb_standby_check(fsm_t fsm_obj);
static fsm_err_t nb_network_check(fsm_t fsm_obj);
static fsm_err_t nb_login_check(fsm_t fsm_obj);
static fsm_err_t nb_login(fsm_t fsm_obj);
static fsm_err_t nb_logout(fsm_t fsm_obj);

// The order of initialization should be consistent with the enumeration definition
static struct fsm_state_class fsm_state_list[FSM_STATE_MAX] =
{
    {   
        ignore_event, \
        nb_power_on, /* pwron event */ \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        "NB Power Off" \
    },
    {   
        nb_power_off, /* pwroff event */ \
        ignore_event, \
        nb_hw_awaken, /* hardware awaken event */ \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        "NB Deep Sleep" \
    },
    {   
        nb_power_off, /* pwroff event */ \
        ignore_event, \
        nb_hw_awaken, /* hardware awaken event */ \
        nb_sw_awaken, /* software awaken event */ \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        "NB Light Sleep" \
    },
    {   
        nb_power_off, /* pwroff event */ \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        nb_standby_check, \
        nb_network_check, /* network check event */ \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        "NB Offline" \
    },
    {   
        nb_power_off, /* pwroff event */ \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        nb_standby_check, /* standby check event */ \
        nb_network_check, /* network check event */ \
        nb_login_check, /* login check event */ \
        nb_login, /* login event */ \
        ignore_event, \
        "NB Online" \
    },
    {   
        nb_power_off, /* pwroff event */ \
        ignore_event, \
        ignore_event, \
        ignore_event, \
        nb_standby_check, /* standby check event */ \
        nb_network_check, /* network check event */ \
        nb_login_check, /* login check event */ \
        ignore_event, \
        nb_logout, /* logout event */ \
        "NB Logged" \
    },
};

static fsm_err_t ignore_event(fsm_t fsm_obj)
{
    printf(">>> The event is meaningless or does not exist in (%s) state\n", fsm_obj->state_obj->usr_data);
    return FSM_EINVALID;
}

static fsm_err_t nb_power_on(fsm_t fsm_obj)
{
    printf(">>> NB device power on\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_OFFLINE];
    return FSM_EOK;
}

static fsm_err_t nb_power_off(fsm_t fsm_obj)
{
    printf(">>> NB device power off\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_PWROFF];
    return FSM_EOK;
}

static fsm_err_t nb_hw_awaken(fsm_t fsm_obj)
{
    printf(">>> NB device hardware awaken\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_OFFLINE];
    return FSM_EOK;
}

static fsm_err_t nb_sw_awaken(fsm_t fsm_obj)
{
    printf(">>> NB device software awaken\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_OFFLINE];
    return FSM_EOK;
}

static fsm_err_t nb_standby_check(fsm_t fsm_obj)
{
    printf(">>> NB device standby check\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_OFFLINE];
    return FSM_EOK;
}

static fsm_err_t nb_network_check(fsm_t fsm_obj)
{
    printf(">>> NB device network check\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_ONLINE];
    return FSM_EOK;
}

static fsm_err_t nb_login_check(fsm_t fsm_obj)
{
    printf(">>> NB device login check\n");
    printf(">>> NB device isn't login\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_ONLINE];
    return FSM_EOK;
}

static fsm_err_t nb_login(fsm_t fsm_obj)
{
    printf(">>> NB device is loging ...\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_LOGGED];
    return FSM_EOK;
}

static fsm_err_t nb_logout(fsm_t fsm_obj)
{
    printf(">>> NB device is log out ...\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_ONLINE];
    return FSM_EOK;
}

static void fsm_err_handler(fsm_err_t err_type)
{
    switch(err_type){
        case FSM_EOK:
            printf(">>> FSM : OK\n");
            break;
        case FSM_ERROR:
            printf(">>> FSM : ERROR\n");
            break;
        case FSM_EINVALID:
            printf(">>> FSM : INVALID ERROR\n");
            break;
        default:
            printf(">>> FSM : BAD ERROR TYPE\n");
    }
}

static struct fsm_class fsm_obj = {&fsm_state_list[FSM_STATE_PWROFF], "NB-IoT Device"};

char *fsm_event_list[] ={
    "PWROFF",
    "PWRON",
    "HW_AWAKEN",
    "SW_AWAKEN",
    "STANDBY_CHECK",
    "NETWORK_CHECK",
    "LOGIN_CHECK",
    "LOGIN",
    "LOGOUT",
};

int main(int agrc, char **argv)
{
    fsm_event_t event;
    fsm_err_t err_type = FSM_ERROR;
    while(1){
        printf("\n========== NB FSM ===========\n");
        printf("| NOW: %s\n", (char *)fsm_obj.state_obj->usr_data);
        printf("|----------------------------\n");
        printf("| EVENT ID LIST:\n");
        int i = FSM_EVENT_PWROFF;
        int j = 0;
        while(i <= FSM_EVENT_LOGOUT){
            printf("|   - %-3d\t%s\n", i, fsm_event_list[j++]);
            i = i << 1;
        }
        printf("|----------------------------\n");
        printf("| INPUT NEW EVENT\n");
        printf("=============================\n");
        scanf("%d", &event);
        switch (event)
        {
            case FSM_EVENT_PWROFF:
                err_type = fsm_obj.state_obj->event_pwroff_handler(&fsm_obj);
                break;
            case FSM_EVENT_PWRON:
                err_type = fsm_obj.state_obj->event_pwron_handler(&fsm_obj);
                break;
            case FSM_EVENT_HW_AWAKEN:
                err_type = fsm_obj.state_obj->event_hw_awaken_handler(&fsm_obj);
                break;
            case FSM_EVENT_SW_AWAKEN:
                err_type = fsm_obj.state_obj->event_sw_awaken_handler(&fsm_obj);
                break;
            case FSM_EVENT_STANDBY_CHECK:
                err_type = fsm_obj.state_obj->event_standby_check_handler(&fsm_obj);
                break;
            case FSM_EVENT_NETWORK_CHECK:
                err_type = fsm_obj.state_obj->event_network_check_handler(&fsm_obj);
                break;
            case FSM_EVENT_LOGIN_CHECK:
                err_type = fsm_obj.state_obj->event_login_check_handler(&fsm_obj);
                break;
            case FSM_EVENT_LOGIN:
                err_type = fsm_obj.state_obj->event_login_handler(&fsm_obj);
                break;
            case FSM_EVENT_LOGOUT:
                err_type = fsm_obj.state_obj->event_logout_handler(&fsm_obj);
                break;
            default:
                printf(">>> BAD EVENT\n");
                break;
        }
        fsm_err_handler(err_type);
    }
    return 0;
}
