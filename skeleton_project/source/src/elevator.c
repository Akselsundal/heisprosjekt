#include <stdio.h>
#include <stdlib.h>
#include "elevator.h"
#include "hardware.h"

void run_elevator(){
    state_t state = boot;
    
    while(1){
        switch(state){
            case boot:
            hardware_init();
            
            state = sleep;
            break;
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













void run_example_program(){
    // Example program
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    hardware_command_movement(HARDWARE_MOVEMENT_UP);

    while(1){
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            break;
        }

        if(hardware_read_floor_sensor(0)){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
    }
}