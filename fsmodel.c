/*
** fsmodel.c
** 2020-07-08
** Remy Shi
** 以事件驱动，整个状态机用事件去回调某个状态下的执行函数（动作或状态切换）。
*/

#include "fsmodel.h"
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

fsm_event_t fsm_event_recv(void)
{
    return FSM_EVENT_1;
}

static fsm_err_t state_a_action_one(fsm_t fsm_obj);
static fsm_err_t state_a_action_two(fsm_t fsm_obj);
static fsm_err_t state_a_action_three(fsm_t fsm_obj);
static fsm_err_t state_b_action_one(fsm_t fsm_obj);
static fsm_err_t state_b_action_two(fsm_t fsm_obj);
static fsm_err_t state_b_action_three(fsm_t fsm_obj);

// The order of initialization should be consistent with the enumeration definition
static struct fsm_state_class fsm_state_list[FSM_STATE_MAX] =
{
     {state_a_action_one, state_a_action_two, state_a_action_three, "STATE A"},
     {state_b_action_one, state_b_action_two, state_b_action_three, "STATE B"},
     {NULL, NULL, NULL, "STATE C"},
};

/* The actions of state A ***********************************/

static fsm_err_t state_a_action_one(fsm_t fsm_obj)
{
    printf(">> Exec state_a_action_one\n");
    return FSM_EOK;
}

static fsm_err_t state_a_action_two(fsm_t fsm_obj)
{
    printf(">> Exec state_a_action_two\n");
    return FSM_EOK;
}

static fsm_err_t state_a_action_three(fsm_t fsm_obj)
{
    printf(">> Exec state_a_action_three\n");
    printf(">> Change state from A to B\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_B];
    return FSM_EOK;
}

/* The actions of state B ***********************************/

static fsm_err_t state_b_action_one(fsm_t fsm_obj)
{
    printf(">> Exec state_b_action_one\n");
    return FSM_EOK;
}

static fsm_err_t state_b_action_two(fsm_t fsm_obj)
{
    printf(">> Exec state_b_action_two\n");
    return FSM_EOK;
}

static fsm_err_t state_b_action_three(fsm_t fsm_obj)
{
    printf(">> Exec state_b_action_three\n");
    printf(">> Change state from B to A\n");
    fsm_obj->state_obj = &fsm_state_list[FSM_STATE_A];
    return FSM_EOK;
}

static void fsm_err_handler(fsm_err_t err_type)
{
    switch(err_type){
        case FSM_EOK:
            printf("(FSM : OK)\n");
            break;
        case FSM_ERROR:
            printf("(FSM : ERROR)\n");
            break;
        case FSM_EINVALID:
            printf("(FSM : INVALID ERROR)\n");
            break;
        default:
            printf("(FSM : BAD ERROR TYPE)\n");
    }
}

int main(int argc, char **argv)
{
    struct fsm_class fsm_obj = {&fsm_state_list[FSM_STATE_A], "FSM Model"};
    int event;
    while(1)
    {
        printf("> Now:%s\n", (char *)fsm_obj.state_obj->usr_data);
        printf("> INPUT NEW EVENT\n");
        fsm_err_t err_type = FSM_ERROR;
        scanf("%d", &event);
        switch (event-1)
        {
            case FSM_EVENT_1:
                err_type = fsm_obj.state_obj->event_1_handler(&fsm_obj);
                break;
            case FSM_EVENT_2:
                err_type = fsm_obj.state_obj->event_2_handler(&fsm_obj);
                break;
            case FSM_EVENT_3:
                err_type = fsm_obj.state_obj->event_3_handler(&fsm_obj);
                break;
            default:
                printf("> BAD EVENT\n");
                break;
        }
        fsm_err_handler(err_type);
    }
    return 0;
}
