#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "timer.h"
#include "elevator.h"
#include "hardware.h"
#include "queue.h"

// Local helper functions
static int transition_state(State *p_now_state, State to_state);

HardwareMovement elevator_movement = HARDWARE_MOVEMENT_UP;
HardwareMovement elevator_last_movement = HARDWARE_MOVEMENT_UP;
int elevator_current_floor = -1.0;  // Initially, unknown floor.
int elevator_next_floor = -1;     // Initially, none.
int elevator_between_floors = 0;


void elevator_boot_state(State *p_now_state){
  if (hardware_read_stop_signal()) {
    transition_state(p_now_state, STOP);
    return;
  }

  if ((elevator_check_and_update_new_floor(&elevator_current_floor)))
    transition_state(p_now_state, IDLE);
}


void elevator_idle_state(State *p_now_state){
  if (hardware_read_stop_signal()) {
    transition_state(p_now_state, STOP);
    return;
  }

  hardware_command_door_open(0);
  queue_check_requests(0, elevator_current_floor);
  elevator_next_floor = queue_get_next_floor(elevator_current_floor, elevator_last_movement);

  if (elevator_next_floor == -1) return;    // Do nothing

  else if (elevator_next_floor > HARDWARE_NUMBER_OF_FLOORS || elevator_next_floor < 0){
    printf("ERROR: The next floor was out of range.\n");
    exit(1);
  }

  else if (hardware_read_obstruction_signal() && hardware_read_floor_sensor(elevator_current_floor)) 
    transition_state(p_now_state, DOORS_OPEN);

  else if(elevator_current_floor == elevator_next_floor){ // A request on the same floor the elevator is on
    transition_state(p_now_state, DOORS_OPEN);
  }

  else 
    transition_state(p_now_state, MOVE);
}


void elevator_move_state(State *p_now_state){
  if (hardware_read_stop_signal()) {
    transition_state(p_now_state, STOP);
    return;
  }

  hardware_command_door_open(0);
  queue_check_requests(1, elevator_current_floor);
  elevator_next_floor = queue_get_next_floor(elevator_current_floor, elevator_last_movement);

  if (elevator_movement == HARDWARE_MOVEMENT_STOP){
    printf("ERROR: The elevator movement was set to stop in 'move' state.\n");
    exit(1);
  }

  elevator_last_movement = elevator_movement;

  if (elevator_check_and_update_new_floor(&elevator_current_floor))
    if (elevator_current_floor == elevator_next_floor)
      transition_state(p_now_state, DOORS_OPEN);

}


void elevator_doors_open_state(State *p_now_state){
  if (hardware_read_stop_signal()) {
    transition_state(p_now_state, STOP);
    return;
  }

  hardware_command_door_open(1);
  queue_remove_requests_on_floor(elevator_current_floor);

  time_t now;
  now = time(NULL);

  while(!timer_check_timeout(now, 3)){
    if (hardware_read_obstruction_signal()){
      now = time(NULL);   // Keep resetting timer
    }
    else{
      queue_check_requests(0, elevator_current_floor);
      elevator_next_floor = queue_get_next_floor(elevator_current_floor, elevator_last_movement);
    }
  }

  
  if (!queue_active_reqs){ //No reqs -> IDLE
    transition_state(p_now_state, IDLE);
    return;
  }

  else if (elevator_next_floor == elevator_current_floor) return;
  else 
    transition_state(p_now_state, MOVE);
}


void elevator_stop_state(State *p_now_state){
  
  hardware_command_door_open(0);
  queue_flush();

  while(hardware_read_stop_signal()){
    hardware_command_stop_light(1);
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    if (hardware_read_floor_sensor(elevator_current_floor)){    // On a floor
      hardware_command_door_open(1);
    }
  }

  hardware_command_stop_light(0);

  time_t prev_time;
  prev_time = time(NULL);

  while(!timer_check_timeout(prev_time, 3)){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    queue_check_requests(0, elevator_current_floor);
  }

  elevator_movement = HARDWARE_MOVEMENT_STOP;

  hardware_command_door_open(0);

  transition_state(p_now_state, IDLE);
}


int elevator_check_and_update_new_floor(int *last_floor){
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if (hardware_read_floor_sensor(i) && *last_floor != i) {
      *last_floor = i;
      hardware_command_floor_indicator_on(i);
      return 1;
    }
  }
  return 0;
}


static int transition_state(State *p_now_state, State to_state){
  if (to_state == IDLE || to_state == DOORS_OPEN || to_state == STOP) elevator_movement = HARDWARE_MOVEMENT_STOP;

  else if (to_state == MOVE && elevator_current_floor < elevator_next_floor) elevator_movement = HARDWARE_MOVEMENT_UP;

  else if (to_state == MOVE && elevator_current_floor > elevator_next_floor) elevator_movement = HARDWARE_MOVEMENT_DOWN;


  int trans_from_boot       = *p_now_state == BOOT        && (to_state == IDLE || to_state == STOP);
  int trans_from_idle       = *p_now_state == IDLE        && (to_state == MOVE || to_state == DOORS_OPEN || to_state == STOP);
  int trans_from_move       = *p_now_state == MOVE        && (to_state == DOORS_OPEN || to_state == STOP);
  int trans_from_doors_open = *p_now_state == DOORS_OPEN  && (to_state == IDLE || to_state == MOVE || to_state == STOP);
  int trans_from_stop       = *p_now_state == STOP        && to_state == IDLE;

  if (trans_from_boot || trans_from_idle || trans_from_move || trans_from_doors_open || trans_from_stop){
    printf("Changing states: %s -> %s\n", elevator_state_strings[*p_now_state], elevator_state_strings[to_state]);
    *p_now_state = to_state;
    return 0;
  }
  else {
    printf("Error: Incorrect transitions of states!\n");
    exit(1);
  }
}


void (*elevator_state_functions[NUMBER_OF_STATES])(State *p_now_state) = {
  elevator_boot_state,
  elevator_idle_state,
  elevator_move_state,
  elevator_doors_open_state,
  elevator_stop_state,
};


char *elevator_state_strings[] = {
  "BOOT",
  "IDLE",
  "MOVE",
  "DOORS OPEN",
  "STOP",
};
