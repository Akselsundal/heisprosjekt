#ifndef ELEVATOR_H
#define ELEVATOR_H

typedef enum {
    boot,
    sleep,
    up,
    down,
    doors_open,
    obstruction,
    stop,
    error
} state_t;


elevator_boot();

void run_example_program();


#endif // ELEVATOR_H
