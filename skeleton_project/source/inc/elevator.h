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


void elevator_run();

void elevator_update_curr_floor(int *last_floor);


// Doxygen

int elevator_transition_state(state_t *p_now_state, state_t to_state, char *msg);


#endif // ELEVATOR_H
