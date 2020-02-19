#ifndef ELEVATOR_H
#define ELEVATOR_H

typedef enum {
    BOOT,
    SLEEP,
    UP,
    DOWN,
    DOORS_OPEN,
    OBSTRUCTION,
    STOP,
    ERROR
} state_t;


state_t elevator_state;
state_t elevator_prev_state;
int current_floor;
char *state_strings[];


void elevator_run();

int elevator_check_new_floor(int *last_floor);


// Doxygen

int elevator_transition_state(state_t *p_now_state, state_t *p_prev_state state_t to_state, char *msg);


#endif // ELEVATOR_H
