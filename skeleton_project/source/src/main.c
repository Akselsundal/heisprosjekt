#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "hardware.h"
#include "example.h"
#include "elevator.h"


static void parse_argvs(int argc, char **argv);

int main(int argc, char **argv){
  parse_argvs(argc, argv);

  printf("Running normal elevator program!\n\n");

  hardware_init();
  queue_init();
  
  state_t state = BOOT;
  HardwareMovement elevator_movement = HARDWARE_MOVEMENT_UP;
  int elevator_current_floor = -1;  // Initially, unknown floor.
  int elevator_next_floor = -1;     // Initially, none.

  // Run program continously
  while(1){

    hardware_command_movement(elevator_movement);
    elevator_state_functions[state](state);

  }

}


static void parse_argvs(int argc, char **argv){
  if (argc < 2) return; // Normal program

  else if (!strcmp(argv[1], "--help")) {
    printf("The following parameters are valid:\n \
            'example' \t\t runs example program\n \
            'test' \t\t runs test program\n");
  }
  else if (!strcmp(argv[1], "example")) run_example();
  else if (!strcmp(argv[1], "test")) return; // Run test program
  else printf("Invalid paramter. Type '--help' for help.\n");
}
