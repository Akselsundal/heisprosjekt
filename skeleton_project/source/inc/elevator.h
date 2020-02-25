/**
*@file elevator.h
*@brief This is the finite state machine, and deals with
*the state of the elevator and the transition between states
*/

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "hardware.h"

#define NUMBER_OF_STATES            6

/**
*@enum An enum to describe which state the elevator is in.
*/
typedef enum {
    BOOT, /*!< The state of the elevator at boot */
    IDLE, /*!< The state when there are no requests */
    MOVE, /*< The elevator is moving */
    DOORS_OPEN, /*!< The elevator doors are open */
    STOP, /*!< Stop is active */
    ERROR, /*!< State to handle errors */
} state_t;


extern void (*elevator_state_functions[NUMBER_OF_STATES])(state_t *p_now_state);
extern char *elevator_state_strings[];

extern HardwareMovement elevator_movement;
extern int elevator_new_floor;
extern int elevator_current_floor;
extern int elevator_next_floor;

void elevator_boot_state(state_t *p_now_state);
void elevator_idle_state(state_t *p_now_state);
void elevator_move_state(state_t *p_now_state);
void elevator_doors_open_state(state_t *p_now_state);
void elevator_stop_state(state_t *p_now_state);
void elevator_error_state(state_t *p_now_state);

#endif // ELEVATOR_H
