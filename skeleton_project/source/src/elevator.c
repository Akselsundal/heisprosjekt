#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "elevator.h"
#include "hardware.h"
#include "queue.h"

static void delay_ms(int number_of_ms);

void elevator_run(){

    hardware_init();
  

    queue_requests = (request_t *) malloc(0);
    queue_length = 0;
    state_t state = sleep;
    int current_floor = elevator_get_curr_floor();

    delay_ms(10000);
  //  int last_floor = current_floor;
    while(1){


      switch(state){

          case sleep:
          transition_from_sleep(&state, current_floor);   // state is a ptr and is updated in the function
          break;

          case up:
          current_floor = elevator_get_curr_floor();
        //  if(current_floor != -1) last_floor = current_floor;
          //printf("Last floor: %i\n", last_floor);

          queue_add_request();


          break;
          case down:
          current_floor = elevator_get_curr_floor();
          break;
          case doors_open:
          break;
          case obstruction:
          break;
          case stop:
          break;
          case error:
          break;
        }
    }
}

int elevator_get_curr_floor(){
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if (hardware_read_floor_sensor(i)){
      return i;
    }
  }
  return -1;
}

int elevator_transition_state(state_t *p_now_state, state_t to_state, char *msg){
  int trans_from_sleep      = *p_now_state == sleep && (to_state == up || to_state == down || to_state == stop);
  int trans_from_up         = *p_now_state == up && (to_state == doors_open || to_state == stop);
  int trans_from_down       = *p_now_state == down && (to_state == doors_open || to_state == stop);
  int trans_from_doors_open = *p_now_state == doors_open && (to_state == sleep || to_state == stop
                               || to_state == up || to_state == down);
  int trans_from_stop       = *p_now_state == stop && to_state == error;
  int trans_from_error      = *p_now_state == error && to_state == sleep;

  if (trans_from_sleep || trans_from_up || trans_from_down || trans_from_doors_open || trans_from_stop || trans_from_error){
    *p_now_state = to_state;
    printf("%s\n", msg);
    return 0;
  }
  else {
    *p_now_state = error;
    printf("Error: Incorrect transitions of states!\n");
    return -1;
  }
}

static void transition_from_sleep(state_t *p_state, int current_floor){
  if (queue_length > -1){
    if(queue_get_next_floor(current_floor, none) > current_floor){
      elevator_transition_state(p_state, up, "Switched from sleep to up state.");
    }
    else if (queue_get_next_floor(current_floor, none) < current_floor){
      elevator_transition_state(p_state, down, "Switched from sleep to down state.");
    }
    else{
      elevator_transition_state(p_state, error, "");
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
