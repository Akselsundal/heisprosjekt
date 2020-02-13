#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hardware.h"
#include "elevator.h"

void parse_argvs(char argc, char **argv);
void run_example_program();

int main(int argc, char **argv){

    parse_argvs(argc, argv);



}










void parse_argvs(char argc, char **argv){
    if (!strcmp(argv[1], "example")){
        run_example_program();
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
