/**
 * @file
 * @brief Time module for delay and timeout functions.
 */

#ifndef TIMER_H
#define TIMER_H

#include <time.h>


extern time_t timer_last_timeout;

/**
 * @brief Keeps the program busy waiting for @p number_of_ms .
 *
 * @param[in] p_from    Source buffer.
 * @param[out] p_to     Destination buffer.
 * @param[in] size      Number of integers in the buffer.
 *
 * @return 0 on success, 1 if either @p p_from or @p p_to
 * is a @c NULL pointer.
 */

int timer_check_timeout(time_t *start_time, const int timeout);

#endif // TIMER_H
