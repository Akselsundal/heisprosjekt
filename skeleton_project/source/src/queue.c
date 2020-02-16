#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int queue_get_next_floor(int current_floor, direction_t dir){
  return 2;

}


void queue_add_request(){
  request_t req;
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_ORDER_DOWN + 1; order++){
      if (hardware_read_order(f, order)){
        req.floor = f;
        req.order_type = order;
        queue_length++;   // realloc fungerer kun for partall queue_length... Spør studass om dette.
        queue_requests = (request_t*)realloc(queue_requests, (sizeof(request_t) * ++queue_length));
        queue_requests[queue_length] = req;
        printf("Added new element to queue:\n");
        printf("Floor: %i\nHardwareOrder: %i\nqueue_length: %i\n", req.floor, req.order_type, queue_length);
      }
    }
  }
}
