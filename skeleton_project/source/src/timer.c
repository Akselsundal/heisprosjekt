#include "timer.h"
#include <time.h>
#include <stdio.h>

int timer_check_timeout(time_t start_time, const int timeout){
  time_t current_time;
  current_time = time(NULL);;

  if (current_time - start_time > timeout){
    return 1;
  }
  return 0;
}

