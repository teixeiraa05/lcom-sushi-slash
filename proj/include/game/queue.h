#ifndef __QUEUE_H
#define __QUEUE_H

#include <lcom/lcf.h>

#define MAX_QUEUE_SIZE 16

typedef struct {
    uint8_t data[MAX_QUEUE_SIZE];
    int front;
    int back;
    int size;
} Queue;


void queue_init(Queue* q);
bool queue_is_empty(const Queue* q);
bool queue_is_full(const Queue* q);
bool queue_push(Queue* q, uint8_t value);
bool queue_pop(Queue* q, uint8_t* value);
bool queue_top(Queue* q, uint8_t* value);
int queue_size(const Queue* q);

#endif // __QUEUE_H
