#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "elevator.h"
#include "hardware.h"
#include "queue.h"

static void delay_ms(int number_of_ms);

static void transition_from_sleep(state_t *p_state, int current_floor);

state_strings = {"BOOT", "SLEEP", "UP", "DOWN", "DOORS_OPEN", "OBSTRUCTION", "STOP", "ERROR"};

void elevator_run(){

  hardware_init();
  queue_init();
  
  state = BOOT;
  prev_state;
  current_floor = -1;  // Initially, unknown floor.

  time_last_timeout = 0;


  delay_ms(10000);
  while(1){

    switch(state){

      case BOOT:
      hardware_command_movement(HARDWARE_MOVEMENT_UP);
      if ((elevator_check_new_floor(&current_floor))) elevator_transition_state(&state, &prev_state, SLEEP, "");
      break;

      case SLEEP:
      hardware_command_movement(HARDWARE_MOVEMENT_STOP);
      queue_add_request();
      transition_from_sleep(&state, current_floor);   // state is a ptr and is updated in the function
      break;

      case UP:
      hardware_command_movement(HARDWARE_MOVEMENT_UP);
      queue_add_request();
      if (elevator_check_new_floor(&current_floor)) 
        elevator_transition_state(&state, &prev_state, DOORS_OPEN, "");
      break;

      case DOWN:
      elevator_update_curr_floor(&current_floor);
      queue_add_request();
      if (elevator_check_new_floor(&current_floor))
        elevator_transition_state(&state, &prev_state, DOORS_OPEN, "");
      break;

      case DOORS_OPEN:
      if (hardware_read_obstruction_signal()) elevator_transition_state(&state, &prev_state, OBSTRUCTION, "");
      queue_add_request();
      if (time_check_timeout(&time_last_timeout, 3000)){
        if(queue_get_next_floor(current_floor, prev_state) > current_floor){
          elevator_transition_state(&state, UP, "");
        }
        else if (queue_get_next_floor(current_floor, prev_state) < current_floor){
          elevator_transition_state(&state, DOWN, "");
        }
      }
      break;

      case OBSTRUCTION:
      queue_flush();
      break;
      case STOP:
      break;
      case ERROR:
      break;
    }
  }
}

int elevator_check_new_floor(int *last_floor){
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if (hardware_read_floor_sensor(i) && *last_floor != i) {
      *last_floor = i;
      return 1;
    }
  }
  return -1;
}

int elevator_transition_state(state_t *p_now_state, state_t *p_prev_state, state_t to_state, char *msg){
  int trans_from_sleep      = *p_now_state == SLEEP       && (to_state == UP || to_state == DOWN || to_state == STOP);
  int trans_from_up         = *p_now_state == UP          && (to_state == DOORS_OPEN || to_state == STOP);
  int trans_from_down       = *p_now_state == DOWN        && (to_state == DOORS_OPEN || to_state == STOP);
  int trans_from_doors_open = *p_now_state == DOORS_OPEN  && (to_state == SLEEP || to_state == STOP
                                                          || to_state == UP || to_state == DOWN);
  int trans_from_stop       = *p_now_state == STOP        && to_state == ERROR;
  int trans_from_error      = *p_now_state == ERROR       && to_state == SLEEP;

  if (trans_from_sleep || trans_from_up || trans_from_down || trans_from_doors_open || trans_from_stop || trans_from_error){
    printf("Changing states: %s -> %s\n", state_strings[*p_now_state], state_strings[*p_prev_state]);
    *p_prev_state = *p_now_state;
    *p_now_state = to_state;

    return 0;
  }
  else {
    *p_prev_state = *p_now_state;
    *p_now_state = ERROR;
    printf("Error: Incorrect transitions of states!\n");
    return -1;
  }
}

static void transition_from_sleep(state_t *p_state, int current_floor){
  if (queue_active_reqs > 0){ // else do not change states
    if(queue_get_next_floor(current_floor, SLEEP) > current_floor){
      elevator_transition_state(p_state, UP, "Switched from sleep to up state.\n");
    }
    else if (queue_get_next_floor(current_floor, SLEEP) < current_floor){
      elevator_transition_state(p_state, DOWN, "Switched from sleep to down state.\n");
    }
    else{
      elevator_transition_state(p_state, ERROR, "");
      printf("Error: Queue module concluded to go to the floor the same floor it already is on.\n");
    }
  }
}

