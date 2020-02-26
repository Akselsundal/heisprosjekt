/**
* @file
* @brief The queue system, handles everything that has to do with the queue.
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"
#include "elevator.h"

#define NUMBER_OF_POSSIBLE_REQUESTS HARDWARE_NUMBER_OF_FLOORS * HARDWARE_NUMBER_OF_MOVEMENT_COMMANDS
/*! \struct request_t
* @brief This struct represents a request.
*/
typedef struct {
/*! Int to describe if the request is active */
  int active;
/*! Describes which floor the request is on */
  int floor;
/*! Describes which type og @p HardwareOrder the request is */
  HardwareOrder order_type;
} request_t;

/** \var queue_requests
* Array of type @p request_t with the size of possible requests, @p NUMBER_OF_POSSIBLE_REQUESTS
*/
request_t queue_requests[NUMBER_OF_POSSIBLE_REQUESTS];

/** \var queue_active_reqs
* Describes the number of active requests in @p queue_requests.
*/
int queue_active_reqs;

/**
* @brief A function to initiate the queue system: sets number of active to 0 and fills @p queue_requests
*/
void queue_init();
void print_active_requests_table();

void queue_number_of_active(); //Rekner ut antall aktive

/**
* @brief A function to find which floor to deal with next.
* @param[in] current_floor The floor where the elevator currently is situated.
* @param[in] state_t The state of which to elevator is currently in.
* @return -1 if @p queue_active_reqs is zero, otherwise which floor to go to.
*/
int queue_get_next_floor(int current_floor);

/**
@breif A function to deal with adding requests.
*/
void queue_add_request();

/**
*@breif A function to deal with removing requests that are dealt with.
*Will set all requests in @p queue_requests on the @p arived_floor to inactive.
*@param[in] arrived_floor The floor which the elevator just arrived.
*/
void queue_remove_requests_on_floor();

/**
*@breif A function to remove all @p request in @p queue_requests to inactive.
*/
void queue_flush();

#endif // QUEUE_H
