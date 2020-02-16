#ifndef ELEVATOR_H
#define ELEVATOR_H

typedef enum {
    sleep,
    up,
    down,
    doors_open,
    obstruction,
    stop,
    error
} state_t;


elevator_boot();

void elevator_run();

int elevator_get_curr_floor();


// Doxygen

int elevator_transition_state(state_t *p_now_state, state_t to_state, char *msg);


#endif // ELEVATOR_H
