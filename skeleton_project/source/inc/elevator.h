/**
*@file elevator.h
*@brief This is the finite state machine, and deals with
*the state of the elevator and the transition between states
*/

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "hardware.h"

#define NUMBER_OF_STATES            5

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
} State;


extern void (*elevator_state_functions[NUMBER_OF_STATES])(State *p_now_state);
extern char *elevator_state_strings[];

extern HardwareMovement elevator_last_movement;
extern HardwareMovement elevator_movement;

extern int elevator_between_floors;
extern int elevator_current_floor;
extern int elevator_next_floor;

void elevator_boot_state(State *p_now_state);
void elevator_idle_state(State *p_now_state);
void elevator_move_state(State *p_now_state);
void elevator_doors_open_state(State *p_now_state);
void elevator_stop_state(State *p_now_state);
int elevator_check_and_update_new_floor(int *last_floor);

#endif // ELEVATOR_H
