/**
 * @file
 * @brief Time module for timeout function.
 */

#ifndef TIMER_H
#define TIMER_H

#include <time.h>

/**
 * @brief Checks if a timeout has occured.
 * @param start_time The time at which the function is first called.
 * @param timeout The number of seconds from when the function is first called to when it
 * returns 1.
 * @return 1 if timeout has occuerd, 0 otherwise.
 */
int timer_check_timeout(time_t start_time, const int timeout);

#endif // TIMER_H