#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
int elevator_boot(){
  hardware_command_movement(HARDWARE_MOVEMENT_UP);
  while(1){
    for (int i = 1 ; i<5 ; i++){
      if (hardware_read_floor_sensor(i)) {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        return i;
      }


    }
  }
}


void run_elevator(){
    hardware_init();
    state = sleep;
    elevator_boot();

    while(1){
        switch(state){
            case sleep:
            break;

            case up:
            break;

            case down:
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



static void clear_all_order_lights();

static void sigint_handler(int sig);

void run_example_program(){}
