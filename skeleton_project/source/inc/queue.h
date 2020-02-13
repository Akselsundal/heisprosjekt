#ifndef QUEUE_H
#define QUEUE_H

typedef enum {
    first = 1, second, thrid, forth
} floor_t;

floor_t floor = first; 

int queue_add_request();


#endif // QUEUE_H