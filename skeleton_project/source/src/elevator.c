#include "timer.h"
#include "elevator.h"
#include "hardware.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


extern HardwareMovement g_movement;

static HardwareMovement elevator_last_movement = HARDWARE_MOVEMENT_UP;
static int elevator_current_floor = -1;
static int elevator_next_floor = -1;
static char *elevator_state_strings[];

static int check_and_update_new_floor(int *last_floor);
static void transition_state(State *p_now_state, State to_state);
static void attempt_doors_open(int door_open);


void elevator_boot_state(State *p_now_state){
    queue_init();
    if (hardware_read_stop_signal()) {
        transition_state(p_now_state, STOP);
        return;
    }

    if ((check_and_update_new_floor(&elevator_current_floor)))
        transition_state(p_now_state, IDLE);
}


void elevator_idle_state(State *p_now_state){
    if (hardware_read_stop_signal()) {
        transition_state(p_now_state, STOP);
        return;
    }

    attempt_doors_open(0);
    queue_check_and_add_requests(elevator_current_floor);
    elevator_next_floor = queue_get_next_floor(elevator_current_floor, elevator_last_movement);
    
    if (!hardware_read_floor_sensor(elevator_current_floor) && elevator_next_floor == elevator_current_floor){
        if(elevator_last_movement == HARDWARE_MOVEMENT_UP) elevator_current_floor++;        // Bit of a hack
        else elevator_current_floor--;                                                      // but it works
        transition_state(p_now_state, MOVE);
        return;
    }

    if (hardware_read_obstruction_signal() && hardware_read_floor_sensor(elevator_current_floor)) 
        transition_state(p_now_state, DOORS_OPEN);

    else if(elevator_current_floor == elevator_next_floor)
        transition_state(p_now_state, DOORS_OPEN);

    else if (elevator_next_floor == -1) return;

    else 
        transition_state(p_now_state, MOVE);
}


void elevator_move_state(State *p_now_state){
    if (hardware_read_stop_signal()) {
        transition_state(p_now_state, STOP);
        return;
    }

    attempt_doors_open(0);
    queue_check_and_add_requests(elevator_current_floor);
    elevator_next_floor = queue_get_next_floor(elevator_current_floor, elevator_last_movement);

    if (g_movement == HARDWARE_MOVEMENT_STOP){
        printf("ERROR: The elevator movement was set to stop in 'move' state.\n");
        exit(1);
    }

    elevator_last_movement = g_movement;

    if (check_and_update_new_floor(&elevator_current_floor))
        if (elevator_current_floor == elevator_next_floor)
        transition_state(p_now_state, DOORS_OPEN);

}


void elevator_doors_open_state(State *p_now_state){
    if (hardware_read_stop_signal()) {
        transition_state(p_now_state, STOP);
        return;
    }

    attempt_doors_open(1);

    time_t time_now;
    time_now = time(NULL);

    while(!timer_check_timeout(time_now, 3)){
        queue_check_and_add_requests(elevator_current_floor);
        elevator_next_floor = queue_get_next_floor(elevator_current_floor, elevator_last_movement);
        queue_remove_requests_on_floor(elevator_current_floor);

        if (hardware_read_stop_signal()) break;

        if (hardware_read_obstruction_signal()) time_now = time(NULL);
    }

    
    if (queue_get_next_floor(elevator_current_floor, elevator_last_movement) == -1){
        transition_state(p_now_state, IDLE);
        return;
    }
    else if (elevator_next_floor == elevator_current_floor) return;
    else transition_state(p_now_state, MOVE);
}


void elevator_stop_state(State *p_now_state){
    
    attempt_doors_open(0);
    queue_flush();

    while(hardware_read_stop_signal()){
        hardware_command_stop_light(1);
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        if (hardware_read_floor_sensor(elevator_current_floor)){
            attempt_doors_open(1);
        }
    }

    hardware_command_stop_light(0);

    time_t prev_time;
    prev_time = time(NULL);

    while(!timer_check_timeout(prev_time, 3)){
        if (hardware_read_stop_signal()) {
            hardware_command_stop_light(1);
            prev_time = time(NULL);
        }
        else hardware_command_stop_light(0);

        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        queue_check_and_add_requests(elevator_current_floor);
    }


    g_movement = HARDWARE_MOVEMENT_STOP;

    attempt_doors_open(0);

    if (!hardware_read_stop_signal()) 
        transition_state(p_now_state, IDLE);
}


static int check_and_update_new_floor(int *last_floor){
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        if (hardware_read_floor_sensor(i) && *last_floor != i) {
            *last_floor = i;
            hardware_command_floor_indicator_on(i);
            return 1;
        }
    }
    return 0;
}


static void transition_state(State *p_now_state, State to_state){
    if (to_state == IDLE || to_state == DOORS_OPEN || to_state == STOP) g_movement = HARDWARE_MOVEMENT_STOP;

    else if (to_state == MOVE && elevator_current_floor < elevator_next_floor) g_movement = HARDWARE_MOVEMENT_UP;

    else if (to_state == MOVE && elevator_current_floor > elevator_next_floor) g_movement = HARDWARE_MOVEMENT_DOWN;


    int trans_from_boot       = *p_now_state == BOOT        && (to_state == IDLE || to_state == STOP);
    int trans_from_idle       = *p_now_state == IDLE        && (to_state == MOVE || to_state == DOORS_OPEN || to_state == STOP);
    int trans_from_move       = *p_now_state == MOVE        && (to_state == DOORS_OPEN || to_state == STOP);
    int trans_from_doors_open = *p_now_state == DOORS_OPEN  && (to_state == IDLE || to_state == MOVE || to_state == STOP);
    int trans_from_stop       = *p_now_state == STOP        &&  to_state == IDLE;

    if (trans_from_boot || trans_from_idle || trans_from_move || trans_from_doors_open || trans_from_stop){
        printf("Changing states: %s -> %s\n", elevator_state_strings[*p_now_state], elevator_state_strings[to_state]);
        *p_now_state = to_state;
    }
    else {
        printf("Error: Incorrect transitions of states!\n");
        exit(1);
    }
}

static void attempt_doors_open(int door_open){
    if (door_open){
        door_open = 0;
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
            if(hardware_read_floor_sensor(i)) door_open = 1;
        }
    }
    hardware_command_door_open(door_open);
}

static char *elevator_state_strings[] = {
    "BOOT",
    "IDLE",
    "MOVE",
    "DOORS OPEN",
    "STOP",
};
