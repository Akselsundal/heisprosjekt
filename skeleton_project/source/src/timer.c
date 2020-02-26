#include "timer.h"
#include <time.h>
#include <stdio.h>

int timer_check_timeout(time_t start_time, const int timeout){
  time_t current_time;
  current_time = time(NULL);
  //time(&current_time);

  time_t diff = current_time - start_time;
  //printf("Diff = %d\t",diff);

  if (diff > timeout){
    //printf("1");
    return 1;
  }
  return 0;
}

