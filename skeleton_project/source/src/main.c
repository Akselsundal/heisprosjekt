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

  elevator_run();

}


static void parse_argvs(int argc, char **argv){
  if (argc < 2) return;// Return to main() for normal program
  else if (!strcmp(argv[1], "--help")) {
    printf("The following parameters are valid:\n \
            'example' \t\t runs example program\n \
            'test' \t\t runs test program\n");
  }
  else if (!strcmp(argv[1], "example")) run_example();
  else if (!strcmp(argv[1], "test")) return; // Run test program
  else printf("Invalid paramter. Type '--help' for help.\n");
}
