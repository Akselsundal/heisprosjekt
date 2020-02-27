#include "queue.h"
#include "elevator.h"
#include "hardware.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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

  for (int i = 0; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){
    if (elevator_movement == HARDWARE_MOVEMENT_UP && (queue_requests[i].order_type == HARDWARE_ORDER_UP || queue_requests[i].order_type == HARDWARE_ORDER_INSIDE)
                                                  && queue_requests[i].active
                                                  && queue_requests[i].floor > current_floor){
      return queue_requests[i].floor;
    }

    else if (elevator_movement == HARDWARE_MOVEMENT_DOWN && (queue_requests[i].order_type == HARDWARE_ORDER_DOWN || queue_requests[i].order_type == HARDWARE_ORDER_INSIDE)
                                                         && queue_requests[i].active
                                                         && queue_requests[i].floor < current_floor){
      return queue_requests[i].floor;

    }
  }
  return 0;
}
//Takes in the already calculated next floor, and the current floor. 
//First it deactivates the calculated next floor, then it sets second_next_floor
//to the new calculated (remember: this function is only called for when there are 
// more than one request.) Then it activates the next floor again, and returns second next.
int queue_get_second_next_floor(int next_floor, int current_floor){
  if (queue_active_reqs < 2) return -1; 
  else{
    for (int f = 0; f < NUMBER_OF_POSSIBLE_REQUESTS; i++){
        if (queue_requests[f].floor = next_floor){
          for (int order = 0 ; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS ; order++){
            if(queue_requests[f*HARDWARE_NUMBER_OF_FLOORS + order].order_type = ){
                queue_requests[f*HARDWARE_NUMBER_OF_FLOORS + order].active = 0
            } 
            elevator_second_next_floor = queue_get_next_floor(current_floor);
            queue_requests[i].active = 1;
            return elevator_second_next_floor;
          }
      }
    }
  }
}

void queue_add_request(){
  int n = 0; //Interator to iterate through all requests.
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
      if (hardware_read_order(f, order) && !queue_requests[f*HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + order].active){
        queue_requests[f*HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + order].active = 1;

        queue_active_reqs = queue_number_of_active();

        
        printf("Added. There are now %i active requests.\n", queue_active_reqs);
        print_active_requests_table();
      }
     
    }
  }
}

void queue_remove_requests_on_floor(int arrived_floor){
  for (int order = 0; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++) {
    queue_requests[arrived_floor*HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + order].active = 0;
    
  }
  queue_active_reqs = queue_number_of_active();
  printf("Removed. There are now %i active requests\n", queue_active_reqs);
  print_active_requests_table();


}

void queue_flush(){
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    queue_remove_requests_on_floor(f);
  }
}




int queue_number_of_active(){
  int sum = 0;
  for (int i = 0 ; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){
    if (queue_requests[i].active) {
      sum++ ;
      }
  }
  

 return sum;
}

void print_active_requests_table(){
  int n  = 0;
  printf("\n============================\n\n");
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    printf("Floor: %i\t", f+1);
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
      printf("%i\t", queue_requests[n].active);
      n++;
    }
    printf("\n");
  }
  printf("\n============================\n\n");
}
