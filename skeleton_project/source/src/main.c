#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "hardware.h"
#include "example.h"
#include "elevator.h"
#include "queue.h"
#include "timer.h"


/*
Known bugs:
- Dersom en etasje bestilles like etter at heisen forlot den, kjører heisen uendelig.
- Dersom stopp trykkes, og en ny bestilling settes på etasjen heisen nettopp var, blir den ikke behandlet.
*/

static time_t watchdog_timer = 0;

static void parse_argvs(int argc, char **argv);
static void sigint_handler(int sig);
static void ping_file(char *byte);

int main(int argc, char **argv){
  parse_argvs(argc, argv);
  signal(SIGINT, sigint_handler);
  printf("Running normal elevator program!\n\n");

  hardware_init();
  queue_init();

  State state = BOOT;
  char byte = 0;

  // Run program continously
  while(1){

    if (timer_check_timeout(watchdog_timer, 1)) {
      ping_file(&byte);
      byte++;
    }
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

static void ping_file(char *byte){
  char *filename = "pingfile.txt";
  int fd = open(filename, O_WRONLY, 0644);

  if (fd < 0) printf("Failed to open %s.\n", filename);
  if (write(fd, byte, sizeof(*byte) < 0)) printf("Failed to write to %s.\n", filename);
  
  close(fd);

  time(&watchdog_timer);
}

static void read_file(char *byte){
  char *filename = "pingfile.txt";
}