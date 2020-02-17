#include "queue.h"
#include "elevator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


static void print_request(const request_t *req);
static void print_all_requests();

int queue_get_next_floor(int current_floor, state_t state){
  request_t *best_request = queue_requests; // First element in queue_requests - iterate over all elements to find best option.

  // Elevator does not priorotize orders from inside; if button was corretly pressed by the passenger from the outside,
  // the direction of the elevator and the passenger's wish will coincide.
  for (int i = 0; i < queue_length; i++){
    if (state == UP && (best_request + i)->floor < best_request->floor){
        best_request = (best_request + i);
    }
    else if (state == DOWN && (best_request + i)->floor > best_request->floor){
        best_request = best_request + i;
    }
  }
  print_all_requests();
  return 0;
}


void queue_add_request(){
  request_t req;
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_ORDER_DOWN + 1; order++){
      if (hardware_read_order(f, order)){
        req.floor = f;
        req.order_type = order;
        queue_length++;
        for (int i = 0; i < queue_length; i++){
          if (queue_requests[i].floor == req.floor && queue_requests[i].order_type == req.order_type){
            break;
            if (i == queue_length -1){
              queue_requests = (request_t*)realloc(queue_requests, (sizeof(request_t) * ++queue_length));
              queue_requests[queue_length] = req;
              printf("A new request was added:\n");
              print_request(&req);
            }
          }
        }

        queue_length--;


        //queue_length++;   // realloc fungerer kun for partall queue_length... Spør studass om dette.
        //queue_requests = (request_t*)realloc(queue_requests, (sizeof(request_t) * ++queue_length));
        //for (int i = 0; i < queue_length; i++)
        //  if (queue_requests[i] == req){
        //    break;
        //    if (i == queue_length - 1) queue_requests[queue_length] = req;
        //  }

        //queue_requests[queue_length] = req;
        //print_request(&req);
      }
    }
  }
}

static void print_request(const request_t *req){
  switch(req->order_type){
    case HARDWARE_ORDER_UP:
    printf("Floor: %i\nOrder type: up\n", req->floor);
    break;
    case HARDWARE_ORDER_INSIDE:
    printf("Floor: %i\nOrder type: inside\n", req->floor);
    break;
    case HARDWARE_ORDER_DOWN:
    printf("Floor: %i\nOrder type: down\n", req->floor);
  }
}

static void print_all_requests(){
  printf("========================================\n\n");
  printf("Printing all requests:\n\n");
  for (int i = 0; i < queue_length; i++){
    printf("Request number %i:\n", i);
    print_request(queue_requests + i);
    printf("\n");
  }
  printf("\n========================================\n\n");
}
