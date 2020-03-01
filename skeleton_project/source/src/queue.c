#include "queue.h"
#include "hardware.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static Request queue_requests[NUMBER_OF_POSSIBLE_REQUESTS];
static int queue_active_requests = 0;

static int check_active_request_on_floor(int floor);


void queue_init(){
    Request req;
    req.active = 0;

    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_N_MOVE_COMMANDS; order++){
            req.floor = f;
            req.order_type = order;
            queue_requests[f * HARDWARE_N_MOVE_COMMANDS + order] = req;
        }
    }
}


void queue_check_and_add_requests(int current_floor){
    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for (HardwareOrder order = HARDWARE_ORDER_UP; order < HARDWARE_N_MOVE_COMMANDS; order++){
            if (hardware_read_order(f, order) && !queue_requests[f * HARDWARE_N_MOVE_COMMANDS + order].active){
                queue_requests[f * HARDWARE_N_MOVE_COMMANDS + order].active = 1;
                hardware_command_order_light(f, order, 1);
                queue_active_requests++;
            }
        }
    }
}


void queue_remove_requests_on_floor(int arrived_floor){
    for (int order = 0; order < HARDWARE_N_MOVE_COMMANDS; order++) {
        if (queue_requests[arrived_floor * HARDWARE_N_MOVE_COMMANDS + order].active){
            queue_requests[arrived_floor * HARDWARE_N_MOVE_COMMANDS + order].active = 0;
            hardware_command_order_light(arrived_floor, order, 0);
            queue_active_requests--;
        }  
    }
}


int queue_get_next_floor(int current_floor, HardwareMovement direction){
    if (!queue_active_requests) return -1;

    if (queue_active_requests == 1)
        for (int i = 0; i < NUMBER_OF_POSSIBLE_REQUESTS; i++)
            if (queue_requests[i].active) return i / HARDWARE_N_MOVE_COMMANDS;


    int up_offset     = 0;
    int inside_offset = 1;
    int down_offset   = 2;

    if (current_floor == 3) direction = HARDWARE_MOVEMENT_DOWN;
    else if (current_floor == 0) direction = HARDWARE_MOVEMENT_UP;

    // Search upwards for requests going in same direction, then search upwards for any requests, then search downwards for any requests
    if (direction == HARDWARE_MOVEMENT_UP){
        
        for (int f = current_floor + 1; f < HARDWARE_NUMBER_OF_FLOORS; f++)
            if (queue_requests[f * HARDWARE_N_MOVE_COMMANDS + up_offset].active || queue_requests[f * HARDWARE_N_MOVE_COMMANDS + inside_offset].active)
                return f;

        for (int f = current_floor + 1; f < HARDWARE_NUMBER_OF_FLOORS; f++)
            if (check_active_request_on_floor(f))
                return f;

        for (int f = current_floor - 1; f >= 0; f--)
            if (check_active_request_on_floor(f))
                return f;

    }

    // Search downwards for requests going in same direction, then search downwards for any requests, then search upwards for any requests
    if (direction == HARDWARE_MOVEMENT_DOWN){
    
        for (int f = current_floor - 1; f >= 0; f--)
            if (queue_requests[f * HARDWARE_N_MOVE_COMMANDS + down_offset].active || queue_requests[f * HARDWARE_N_MOVE_COMMANDS + inside_offset].active)
                return f;

        for (int f = current_floor - 1; f >= 0; f--)
            if (check_active_request_on_floor(f))
                return f;

        for (int f = current_floor + 1; f < HARDWARE_NUMBER_OF_FLOORS; f++)
            if (check_active_request_on_floor(f))
                return f;

    }

    return -1;

}


void queue_flush(){
    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        queue_remove_requests_on_floor(f);
    }
}


static int check_active_request_on_floor(int floor){
    for (HardwareOrder order = HARDWARE_MOVEMENT_UP; order < HARDWARE_N_MOVE_COMMANDS; order++){
        if (queue_requests[floor * HARDWARE_N_MOVE_COMMANDS + order].active) return 1;
    }
    return 0;
}