/**
*@file elevator.h
*@brief This is the finite state machine, and deals with
*the state of the elevator and the transition between states
*/

#ifndef ELEVATOR_H
#define ELEVATOR_H

/**
*@enum An enum to describe which state the elevator is in.
*/
typedef enum {
    BOOT, /*!< The state of the elevator at boot */
    SLEEP, /*!< The state when there are no requests */
    UP, /*!< The elevator is going up state */
    DOWN, /*!< The elevator is going down state */
    DOORS_OPEN, /*!< The elevator doors are open */
    OBSTRUCTION, /*!< There is an obstruction! */
    STOP, /*!< Stop is active */
    ERROR /*!< State to handle errors */
} state_t;

/**
*
*/
state_t elevator_state;
state_t elevator_prev_state;
int current_floor;
char *state_strings[];


void elevator_run();

int elevator_check_new_floor(int *last_floor);


// Doxygen

int elevator_transition_state(state_t *p_now_state, state_t *p_prev_state state_t to_state, char *msg);


#endif // ELEVATOR_H
