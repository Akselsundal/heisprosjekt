/**
*@file elevator.h
*@brief This file contains a function pointer array and the five functions in it. Along with
*@p State enum, this summarizes the finite state machine.
*/
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "hardware.h"

#define NUMBER_OF_STATES                                5

/**
*@enum An enum to describe which state the elevator is in.
*/
typedef enum {
    BOOT,       /*!< The state of the elevator at boot */
    IDLE,       /*!< The state when there are no requests */
    MOVE,       /*< The elevator is moving */
    DOORS_OPEN, /*!< The elevator doors are open */
    STOP,       /*!< Stop is active */
    ERROR,      /*!< State to handle errors */
} State;

/** @brief Function pointer array of five elements, which returns nothing and takes in a pointer
 * to the finite state machine's state.
 */
void (*g_state_functions[NUMBER_OF_STATES])(State *p_now_state);

/** @brief Stores the movement of the elevator. */
HardwareMovement g_movement;

/**
 * @brief Contains everything that will happen in the @p BOOT state.
 * @param p_now_state A pointer to the current state.
 */
void elevator_boot_state(State *p_now_state);

/**
 * @brief Contains everything that will happen in the @p IDLE state.
 * @param p_now_state A pointer to the current state.
 */
void elevator_idle_state(State *p_now_state);

/**
 * @brief Contains everything that will happen in the @p MOVE state.
 * @param p_now_state A pointer to the current state.
 */
void elevator_move_state(State *p_now_state);

/**
 * @brief Contains everything that will happen in the @p DOORS_OPEN state.
 * @param p_now_state A pointer to the current state.
 */
void elevator_doors_open_state(State *p_now_state);

/**
 * @brief Contains everything that will happen in the @p STOP state.
 * @param p_now_state A pointer to the current state.
 */
void elevator_stop_state(State *p_now_state);

#endif // ELEVATOR_H
