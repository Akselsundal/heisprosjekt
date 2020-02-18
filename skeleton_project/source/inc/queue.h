#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"
#include "elevator.h"

#define NUMBER_OF_POSSIBLE_REQUESTS     HARDWARE_NUMBER_OF_FLOORS * 3   // Except for two options that are not possible in hardware

typedef struct {
  int active;
  int floor;
  HardwareOrder order_type;
} request_t;


request_t queue_requests[NUMBER_OF_POSSIBLE_REQUESTS];
int queue_active_reqs;

void queue_init();
int queue_get_next_floor(int current_floor, state_t state, request_t *request);
void queue_add_request();
void queue_remove_request();
void queue_flush();

#endif // QUEUE_H
