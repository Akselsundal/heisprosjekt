/**
* @file
* @brief This file contains functions for initializing the queue, adding and removing requests,
finding the best floor to go to, and flushing the entire queue.
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"

#define NUMBER_OF_POSSIBLE_REQUESTS        HARDWARE_NUMBER_OF_FLOORS * HARDWARE_N_MOVE_COMMANDS

/*! \struct Request
* @brief This struct represents a request.
*/
typedef struct {
    int active;                 /*! Describes whether the request is active or not */
    int floor;                  /*! Describes which floor the request is on */
    HardwareOrder order_type;   /*! Describes which type of @p HardwareOrder the request is */
} Request;

/**
* @brief A function to initiate the queue system: sets number of active to 0 and fills @p queue_requests
* with inactive requests.
* @warning There will be 12 elements, but in hardware there are only 10 possible types of requests.
* The two remaining elements will never be active.
*/
void queue_init();

/**
 * @brief Checks new requests and adds them if they were not already active. Increments @p queue_active_reqs by one
 * if a new request is added.
 * @param current_floor The floor the elevator currently is on.
 */
void queue_check_and_add_requests(int current_floor);

/**
* @brief Removes all active requests on a specified floor. Decrements @p queue_active_reqs by one for each
* active request was removed on that floor.
* @param arrived_floor The floor which the elevator just arrived.
*/
void queue_remove_requests_on_floor(int arrived_floor);

/**
* @brief Determines which floor the elevator will go to next.
* @param current_floor The floor the elevator currently is on.
* @param direction The general direction of the elevator. This value should never be @p HARDWARE_MOVEMENT_STOP .
* @return -1 if @p queue_active_reqs is zero, otherwise which floor to go to.
*/
int queue_get_next_floor(int current_floor, HardwareMovement direction);


/**
* @brief Removes all active requests and sets @p queue_active_reqs to zero.
*/
void queue_flush();

#endif // QUEUE_H
