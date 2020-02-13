#include <stdio.h>
#include <stdlib.h>
#include "boot.h"
#include "hardware.h"

int set(){
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
