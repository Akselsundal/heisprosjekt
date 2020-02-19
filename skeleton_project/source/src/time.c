#include <time.h>


void time_delay_ms(int number_of_ms){
    return;
}

int time_check_timeout(time_t *prev_time, const int timeout){
    time_t now_time = 0;
    time(&now_time);
    if (now_time - *prev_time > timeout){
        *prev_time = now_time;
        return 1;
    }
    return 0;
}