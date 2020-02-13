#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "elevator.h"
#include "boot.h"
#include "example.h"

void parse_argvs(int argc, char **argv);

int main(int argc, char **argv){

    parse_argvs(argc, argv);

    return 0;
}

void parse_argvs(int argc, char **argv){
    if (argc > 1){
        if (!strcmp(argv[1], "example")){
            elevator_run();
        }
        /*else if (!strcmp(argv[1], "test")){

        }*/
        else{
            printf("Invalid usage. Type elevator --help for correct usage.\n");
        }
    }
    else 

}