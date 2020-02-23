#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include "hardware.h"
#include "example.h"
#include "elevator.h"
#include "queue.h"
#include "timer.h"


static void parse_argvs(int argc, char **argv);
static void sigint_handler(int sig);

int main(int argc, char **argv){
  parse_argvs(argc, argv);
  signal(SIGINT, sigint_handler);
  printf("Running normal elevator program!\n\n");

  hardware_init();
  queue_init();

  state_t state = BOOT;

  // Run program continously
  while(1){

    hardware_command_movement(elevator_movement);
    elevator_state_functions[state](&state);

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


static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}
