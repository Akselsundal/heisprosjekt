#include <time.h>


void time_delay_ms(int number_of_ms){
    return;
}

int time_check_timeout(time_t *prev_time, const time_t *timeout){
    time_t *now_time;
    time(now_time);
    if (*now_time - *prev_time > *timeout){
        *prev_time = *now_time;
        return 1;
    }
    return 0;
}