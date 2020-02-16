#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "elevator.h"
#include "hardware.h"
#include "queue.h"

static void delay_ms(int number_of_ms);

void elevator_run(){

    state_t state = BOOT;
    int current_floor = -1;  // Initially, unknown and invalid floor.

    queue_requests = (request_t *) malloc(0);   // Initilize dynamic queue array
    queue_length = 0;

    hardware_init();

    delay_ms(10000);
    while(1){

      switch(state){

          case BOOT:

          hardware_command_movement(HARDWARE_MOVEMENT_UP);
          elevator_update_curr_floor(&current_floor);
          if (current_floor != -1) state = SLEEP;

          case SLEEP:
          hardware_command_movement(HARDWARE_MOVEMENT_STOP);
          transition_from_sleep(&state, current_floor);   // state is a ptr and is updated in the function
          break;

          case UP:
          elevator_update_curr_floor(current_floor);
          queue_add_request();


          break;
          case DOWN:
          elevator_update_curr_floor(current_floor);
          queue_add_request();
          break;
          case DOORS_OPEN:
          queue_add_request();
          break;
          case OBSTRUCTION:
          break;
          case STOP:
          break;
          case ERROR:
          break;
        }
    }
}

void elevator_update_curr_floor(int *last_floor){
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if (hardware_read_floor_sensor(i)) *last_floor = i;
  }
}

int elevator_transition_state(state_t *p_now_state, state_t to_state, char *msg){
  int trans_from_sleep      = *p_now_state == SLEEP       && (to_state == UP || to_state == DOWN || to_state == STOP);
  int trans_from_up         = *p_now_state == UP          && (to_state == DOORS_OPEN || to_state == STOP);
  int trans_from_down       = *p_now_state == DOWN        && (to_state == DOORS_OPEN || to_state == STOP);
  int trans_from_doors_open = *p_now_state == DOORS_OPEN  && (to_state == SLEEP || to_state == STOP
                                                          || to_state == UP || to_state == DOWN);
  int trans_from_stop       = *p_now_state == STOP        && to_state == ERROR;
  int trans_from_error      = *p_now_state == ERROR       && to_state == SLEEP;

  if (trans_from_sleep || trans_from_up || trans_from_down || trans_from_doors_open || trans_from_stop || trans_from_error){
    *p_now_state = to_state;
    printf("%s\n", msg);
    return 0;
  }
  else {
    *p_now_state = ERROR;
    printf("Error: Incorrect transitions of states!\n");
    return -1;
  }
}

static void transition_from_sleep(state_t *p_state, int current_floor){
  if (queue_length > 0){ // else do not change states
    if(queue_get_next_floor(current_floor, none) > current_floor){
      elevator_transition_state(p_state, UP, "Switched from sleep to up state.");
    }
    else if (queue_get_next_floor(current_floor, none) < current_floor){
      elevator_transition_state(p_state, DOWN, "Switched from sleep to down state.");
    }
    else{
      elevator_transition_state(p_state, ERROR, "");
      printf("Error: Queue module concluded to go to the floor the ");
      printf("elevator already is on.\n");
    }
  }
}

static void transtition_from_up(state_t state){

}


static void delay_ms(int number_of_ms){
  time_t *time_now;
  time_t *next_time;
  time(time_now);
  while (*next_time < *time_now + number_of_ms){
    time(next_time);
  }
}
