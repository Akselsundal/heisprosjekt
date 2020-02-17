/**
 * @file
 * @brief Time module for delay and timeout functions.
 */

#ifndef TIME_H
#define TIME_H

#include <time.h>



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
void time_delay_ms(int number_of_ms);

int time_check_timeout(time_t *prev_time, time_t *timeout);

#endif // TIME_H