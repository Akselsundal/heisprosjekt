#include "queue.h"
#include "elevator.h"
#include "hardware.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void print_request(const request_t *req);
static void print_all_requests();


void queue_init(){
  queue_active_reqs = 0;
  int n = 0;
  request_t req;
  req.active = 0;
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
      req.floor = f;
      req.order_type = order;
      queue_requests[n] = req;
      n++;
    }
  }
}

int queue_get_next_floor(int current_floor){
  if (!queue_active_reqs) return -1;  // No elements in queue

  // If there is only one active element, return its floor
  // else set request to first active element
  for (int i = 0; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){
    if (queue_active_reqs == 1 && queue_requests[i].active){
        return queue_requests[i].floor;
    }
  }

  int smallest_diff = HARDWARE_NUMBER_OF_FLOORS - 1;    // Start with worst case and find better options

  for (int i = 0; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){


    int diff = queue_requests[i].floor - current_floor;
    //if (queue_requests[i].order_type == HARDWARE_ORDER_INSIDE)
    // Case 1: elevator going up, request from above, request going up.
    // Priority number 1.
    if (elevator_movement == HARDWARE_MOVEMENT_UP && queue_requests[i].order_type == HARDWARE_ORDER_UP
                                                  && queue_requests[i].active
                                                  && queue_requests[i].floor > current_floor){
      return queue_requests[i].floor;


    //  if (diff < smallest_diff){
      //  smallest_diff = diff;
      //}

    }
    else if (elevator_movement == HARDWARE_MOVEMENT_DOWN && queue_requests[i].order_type == HARDWARE_ORDER_DOWN
                                                         && queue_requests[i].active
                                                         && queue_requests[i].floor < current_floor){

      if (diff > smallest_diff){
        smallest_diff = diff;
      }

    }
  }
  return elevator_current_floor + smallest_diff;
}

void queue_add_request(){
  int n = 0; //Interator to iterate through all requests
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
      if (hardware_read_order(f, order) && !queue_requests[n].active){
        queue_requests[n].active = 1;
        queue_active_reqs++;

        printf("A request was added:");
        print_request(&queue_requests[n]);
        printf("There are now %i active requests.\n", queue_active_reqs);
      }
      n++;
    }
  }
}

int queue_remove_requests_on_floor(int arrived_floor){
  if (!queue_active_reqs) return 0;

  int removed_reqs = 0;


  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    if (queue_requests[f].floor == arrived_floor){
      for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
        if (queue_requests[f*HARDWARE_NUMBER_OF_FLOORS + order].active) {
          removed_reqs = 1;
          queue_requests[f*HARDWARE_NUMBER_OF_FLOORS + order].active = 0;
          printf("removed request (this is inside loop)\n");
        }
        }

      }
    }

  //queue_active_reqs -= number_of_removed_reqs;
  return removed_reqs;
}

void queue_flush(){
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    queue_remove_requests_on_floor(f);
  }
}


static void print_request(const request_t *req){
  printf("\n---------------------------\n\n");
  switch(req->order_type){
    case HARDWARE_ORDER_UP:
    printf("Floor:\t\t%i\nOrder type:\tup\n", req->floor + 1);
    break;
    case HARDWARE_ORDER_INSIDE:
    printf("Floor:\t\t%i\nOrder type:\tinside\n", req->floor + 1);
    break;
    case HARDWARE_ORDER_DOWN:
    printf("Floor:\t\t%i\nOrder type:\tdown\n", req->floor + 1);
  }
  printf("\n---------------------------\n\n");
}

static void print_all_requests(){
  printf("\n========================================\n\n");
  printf("Printing all requests:\n\n");
  for (int i = 0; i < queue_active_reqs; i++){
    printf("Request number %i:\n", i + 1);
    print_request(&queue_requests[i]);
  }
  printf("========================================\n\n");
}

void print_active_requests_table(){
  printf("\n============================\n\n");
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
      printf("%i\t", queue_requests[f * HARDWARE_NUMBER_OF_FLOORS + order].active);
    }
    printf("\n");
  }
  printf("\n============================\n\n");
}
