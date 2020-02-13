#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "elevator.h"

void parse_argvs(char argc, char **argv);

int main(char argc, char **argv){

    parse_argvs(argc, argv);



}

void parse_argvs(char argc, char **argv){
    if (strcmp(argv[1]), "example"){
        run_example_program();
    }


}