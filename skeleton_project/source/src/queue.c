#include "queue.h"
#include "elevator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void print_request(const request_t *req);
static void print_all_requests();
static int compare_requests(const request_t req1, const request_t req2);


void queue_init(){
  queue_active_reqs = 0;

  request_t req;
  req.active = 0;
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_ORDER_DOWN + 1; order++){
      req.floor = f;
      req.order_type = order;
      queue_requests[f * HARDWARE_NUMBER_OF_FLOORS + order] = req;
    }
  }
}

int queue_get_next_floor(int current_floor, state_t state){
  if (!queue_active_reqs) return -1;  // No elements in queue

  request_t request;

  // Identify first active element in queue
  int i = 0;
  for (; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){
    if (queue_requests[i].active){
      request = queue_requests[i];
      break;
    }
  }

  // Determine best active element in queue
  for (int j = i + 1; j < NUMBER_OF_POSSIBLE_REQUESTS; j++){
    if (queue_requests[j].active){
      if (state == UP && queue_requests[i].order_type == HARDWARE_ORDER_UP 
          && queue_requests[j].floor > current_floor && queue_requests[j].floor < request.floor){

        request = queue_requests[j];

      }
      else if (state == DOWN && queue_requests[i].order_type == HARDWARE_ORDER_DOWN
          && queue_requests[j].floor < current_floor && queue_requests[j].floor > request.floor){

        request = queue_requests[j];

      }
    }
  }
  print_all_requests();
  return 0;
}


int queue_add_request(){
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_ORDER_DOWN + 1; order++){
      if (hardware_read_order(f, order) && !queue_requests[f * HARDWARE_NUMBER_OF_FLOORS + order].active){
        queue_requests[f * HARDWARE_NUMBER_OF_FLOORS + order].active = 1;
        queue_active_reqs++;

        printf("A request was added:\n")
        print_request(&queue_requests[f * HARDWARE_NUMBER_OF_FLOORS + order]);
        return 0;
      }
    }
  }
  return -1;
}

void queue_remove_requests_on_floor(int arrived_floor){
  if (!queue_active_reqs) return -1;

  printf("Attempting to remove request on floor %i:\n", arrived_floor);

  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if (queue_requests[i * (HARDWARE_NUMBER_OF_FLOORS - 1)].floor == arrived_floor){
      // Set all requests on arrived floor to inactive
      for (int j = 0; j < 3; j++){ // Magic number
        queue_requests[i + j].active = 0;
      }
    }
  }
}

void queue_flush(){
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    queue_remove_requests_on_floor(f);
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
  for (int i = 0; i < queue_active_reqs; i++){
    printf("Request number %i:\n", i);
    print_request(queue_requests + i);
    printf("\n");
  }
  printf("\n========================================\n\n");
}

static int compare_requests(const request_t req1, const request_t req2){
  if (req1.active == req2.active && req1.floor == req2.floor && req1.order_type == req2.order_type) return 1;
  return 0;
}
