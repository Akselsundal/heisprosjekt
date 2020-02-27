#include "queue.h"
#include "hardware.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void print_active_requests_matrix();


void queue_init(){
  queue_active_reqs = 0;
  Request req;
  req.active = 0;
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
      req.floor = f;
      req.order_type = order;
      queue_requests[f * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + order] = req;
    }
  }
}


void queue_check_requests(int between_floors, int current_floor){
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
      if (between_floors && f == current_floor) return;
  
      if (hardware_read_order(f, order) && !queue_requests[f*HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + order].active){
        queue_requests[f*HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + order].active = 1;
        hardware_command_order_light(f, order, 1);
        queue_active_reqs++;
      }
    }
  }
}


void queue_remove_requests_on_floor(int arrived_floor){
  for (int order = 0; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++) {
    if (queue_requests[arrived_floor * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + order].active){
      queue_requests[arrived_floor*HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + order].active = 0;
      hardware_command_order_light(arrived_floor, order, 0);
      queue_active_reqs--;
    }  
  }
}


int queue_get_next_floor(int current_floor, HardwareMovement last_movement){
  if (!queue_active_reqs) return -1;  // No elements in queue

  // If there is only one active element, return its floor
  if (queue_active_reqs == 1){
    for (int i = 0; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){
      if (queue_requests[i].active){
        return queue_requests[i].floor;
      }
    }
  }

  if (last_movement == HARDWARE_MOVEMENT_UP){
    for (int i = current_floor * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){
      if (queue_requests[i].active && (queue_requests[i].order_type == HARDWARE_ORDER_UP || queue_requests[i].order_type == HARDWARE_ORDER_INSIDE)){
        return queue_requests[i].floor;
      }
    }
    for (int i = current_floor * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){
      if (queue_requests[i].active){
        return queue_requests[i].floor;
      }
    }
    for (int i = current_floor * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + 2; i > 0; i--){
      if (queue_requests[i].active){
        return queue_requests[i].floor;
      }
    }
  }

  if (last_movement == HARDWARE_MOVEMENT_DOWN){    // Kommenter eller forklar 2-tallet.
    for (int i = current_floor * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + 2; i > 0; i--){
      if (queue_requests[i].active && (queue_requests[i].order_type == HARDWARE_ORDER_DOWN || queue_requests[i].order_type == HARDWARE_ORDER_INSIDE)){
        return queue_requests[i].floor;
      }
    }
    for (int i = current_floor * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS + 2; i > 0; i--){
      if (queue_requests[i].active){
        return queue_requests[i].floor;
      }
    }
    for (int i = current_floor * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; i < NUMBER_OF_POSSIBLE_REQUESTS; i++){
      if (queue_requests[i].active){
        return queue_requests[i].floor;
      }
    }
  }

  printf("queue_get_next_floor() did not find any floor to go to.\n");

  return -1;
}


void queue_flush(){
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    queue_remove_requests_on_floor(f);
  }
}


static void print_active_requests_matrix(){
  printf("\n============================\n\n");
  for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    printf("Floor: %i\t", f+1);
    for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS; order++){
      printf("%i\t", queue_requests[order].active);
    }
    printf("\n");
  }
  printf("\n============================\n\n");
}
