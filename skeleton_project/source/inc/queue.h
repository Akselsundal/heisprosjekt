#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"
#include "elevator.h"


typedef struct {
  int floor;
  HardwareOrder order_type;
} request_t;


request_t *queue_requests;
int queue_length;


int queue_get_next_floor(int current_floor, state_t state);
void queue_add_request();

#endif // QUEUE_H
