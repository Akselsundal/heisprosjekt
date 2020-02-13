#ifndef QUEUE_H
#define QUEUE_H

#include "elevator.h"
#include "hardware.h"

typedef enum floor{
    first = 1, second, third, forth
} floor_t;

floor_t queue_curr_floor;
floor_t queue_commands[HARDWARE_NUMBER_OF_FLOORS];

int queue_add_request(floor_t req_floor, state_t curr_state);
int queue_remove_request();
int queue_flush();
int queue_get_next_floot();



#endif