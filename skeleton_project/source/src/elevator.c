#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "timer.h"
#include "elevator.h"
#include "hardware.h"
#include "queue.h"

// Local helper functions
static int check_new_floor(int *last_floor);
static int transition_state(state_t *p_now_state, state_t to_state);

HardwareMovement elevator_movement = HARDWARE_MOVEMENT_UP;
int elevator_current_floor = -1;  // Initially, unknown floor.
int elevator_next_floor = -1;     // Initially, none.
int elevator_second_next_floor = -1; // The floor the elevator will go to second. 


void elevator_boot_state(state_t *p_now_state){
  if ((check_new_floor(&elevator_current_floor) != -1)) {
    //Oppdatere el_current_floor her? 
    transition_state(p_now_state, IDLE);
  }
}

void elevator_idle_state(state_t *p_now_state){
  queue_add_request();

  elevator_next_floor = queue_get_next_floor(elevator_current_floor);

  if (elevator_next_floor == -1) return;    // Do nothing

  else if(elevator_current_floor == elevator_next_floor){ // A request on the same floor the elevator is on
    transition_state(p_now_state, DOORS_OPEN);

  }

  else{
    transition_state(p_now_state, MOVE);

  }
}

void elevator_move_state(state_t *p_now_state){
  queue_add_request();
  elevator_next_floor = queue_get_next_floor(elevator_current_floor);
  elevator_second_next_floor = queue_get_second_next_floor(elevator_next_floor, elevator_current_floor); //-1 if there is only one request. 

  
  
  if (check_new_floor(&elevator_current_floor)){
    if (elevator_current_floor == elevator_next_floor){
      
      transition_state(p_now_state, DOORS_OPEN);

    }
  }
}

void elevator_doors_open_state(state_t *p_now_state){
  queue_remove_requests_on_floor(elevator_current_floor);
  time_t now;
  now = time(NULL);
  //time(timer_last_timeout);

  while (!timer_check_timeout(now, 3)){
    queue_add_request();
  
  }
  if (!queue_active_reqs){ //No reqs -> IDLE
    transition_state(p_now_state, IDLE);
    return;

  }

  else if (elevator_second_next_floor == -1) {
    elevator_next_floor = queue_get_next_floor(elevator_current_floor);
    if (elevator_next_floor == -1) {
      transition_state(p_now_state, IDLE);
    }
    else{
      transition_state(p_now_state, MOVE);

    }
    
  }

   else{ 
    // More than one req? 
    //    Case 1, There were more than one before DOOR_OPEN(while MOVEing):
    //                    This will hopefully be saved in the second_next_floor, so just set next_floor
    //                    to second_next_floor
    //    Case 2, The requests were added after doors opened!: 
    //              second_next_floor will be -1 and get_next_floor proceedes as usual!
    
    if (elevator_second_next_floor == -1){
      elevator_next_floor = queue_get_next_floor(elevator_current_floor);
      printf("ELEVATOR NEXT FLOOR IS %i\n", elevator_next_floor);
      transition_state(p_now_state, MOVE);
    }

    else{
      printf("ELEVATOR SECOND NEXT FLOOR IS %i\n", elevator_second_next_floor);
      elevator_next_floor = elevator_second_next_floor;
      transition_state(p_now_state, MOVE);
    }
    

    
}


 
  
  
}




void elevator_stop_state(state_t *p_now_state){
  // Run code for each state here.
}
void elevator_error_state(state_t *p_now_state){
  // Run code for each state here.
}

//
static int check_new_floor(int *last_floor){
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if (hardware_read_floor_sensor(i) && *last_floor != i) {
      *last_floor = i;
      return 1;
    }
  }
  return -1;
}

static int transition_state(state_t *p_now_state, state_t to_state){

if (to_state == IDLE || to_state == DOORS_OPEN || to_state == STOP){
  elevator_movement = HARDWARE_MOVEMENT_STOP;
}

if (to_state == MOVE){
  if (elevator_next_floor > elevator_current_floor && elevator_current_floor != HARDWARE_NUMBER_OF_FLOORS){
    elevator_movement = HARDWARE_MOVEMENT_UP;

    }

  if(elevator_next_floor < elevator_current_floor && elevator_current_floor != 0){
    elevator_movement = HARDWARE_MOVEMENT_DOWN;

    }
}


  int trans_from_boot       = *p_now_state == BOOT        && to_state == IDLE;
  int trans_from_idle       = *p_now_state == IDLE        && (to_state == MOVE || to_state == STOP || to_state == DOORS_OPEN);
  int trans_from_move       = *p_now_state == MOVE        && (to_state == DOORS_OPEN || to_state == STOP);
  int trans_from_doors_open = *p_now_state == DOORS_OPEN  && (to_state == IDLE || to_state == STOP || to_state == MOVE);
  int trans_from_stop       = *p_now_state == STOP        && to_state == ERROR;
  int trans_from_error      = *p_now_state == ERROR       && to_state == IDLE;

  if (trans_from_boot || trans_from_idle || trans_from_move || trans_from_doors_open || trans_from_stop || trans_from_error){
    printf("Changing states: %s -> %s\n", elevator_state_strings[*p_now_state], elevator_state_strings[to_state]);
    *p_now_state = to_state;
    return 0;
  }
  else {
    *p_now_state = ERROR;
    printf("Error: Incorrect transitions of states!\n");
    return -1;
  }
}


void (*elevator_state_functions[NUMBER_OF_STATES])(state_t *p_now_state) = {
  elevator_boot_state,
  elevator_idle_state,
  elevator_move_state,
  elevator_doors_open_state,
  elevator_stop_state,
  elevator_error_state,
};

char *elevator_state_strings[] = {
  "BOOT",
  "IDLE",
  "MOVE",
  "DOORS OPEN",
  "STOP",
  "ERROR",
};
