#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "hardware.h"
#include "elevator.h"


void (*state_functions[NUMBER_OF_STATES])(State *p_now_state);
HardwareMovement g_movement = HARDWARE_MOVEMENT_UP;

static void sigint_handler(int sig);

int main(){
    signal(SIGINT, sigint_handler);
    printf("Running normal elevator program!\n\n");

    hardware_init();

    State state = BOOT;

    while(1){
        hardware_command_movement(g_movement);
        state_functions[state](&state);
    }
}

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

void (*state_functions[NUMBER_OF_STATES])(State *p_now_state) = {
    elevator_boot_state,
    elevator_idle_state,
    elevator_move_state,
    elevator_doors_open_state,
    elevator_stop_state,
};