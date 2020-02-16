#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"

int queue_length;

typedef struct {
  int floor;
  HardwareOrder order_type;
} request_t;

request_t *queue_requests;

typedef enum {
  dir_up, dir_down, none
} direction_t;

int queue_get_next_floor(int current_floor, direction_t dir);
void queue_add_request();

#endif // QUEUE_H
