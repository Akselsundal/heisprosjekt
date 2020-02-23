#include <time.h>
#include <stdio.h>

int timer_check_timeout(time_t *start_time, const int timeout){
  time_t current_time;
  time(&current_time);

  time_t diff = current_time - *start_time;

  if (diff > timeout){
    return 1;
  }
  return 0;
}
