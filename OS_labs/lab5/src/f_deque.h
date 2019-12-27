#ifndef OS_LAB5_F_DEQUE_H
#define OS_LAB5_F_DEQUE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct node{
    struct node* next;
    struct node* prev;
    double data;
} node;

typedef struct f_deque {
    node* head_left;
    node* head_right;
    size_t size;
} f_deque;

#endif //OS_LAB5_F_DEQUE_H
