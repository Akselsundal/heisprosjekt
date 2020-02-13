#ifndef QUEUE_H
#define QUEUE_H

typedef enum {
    first = 1, second, thrid, forth
} floor_t;

floor_t queue_curr_floor;





int queue_add_request();


#endif // QUEUE_H