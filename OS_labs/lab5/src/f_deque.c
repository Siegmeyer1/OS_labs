#include "f_deque.h"

f_deque* create(){
    f_deque* deque = (f_deque*)malloc(sizeof(f_deque));
    deque->head_left = NULL;
    deque->head_right = NULL;
    deque->size = 0;
    return deque;
}

_Bool empty(f_deque* deque){
    if(deque->size == 0)
        return 1;
    else
        return 0;
}

void push_back(f_deque* deque, double flt){
    node* newnode = (node*)malloc(sizeof(node));
    newnode->next = NULL;
    newnode->data = flt;
    if(empty(deque)){
        deque->head_left = newnode;
        deque->head_right = newnode;
        newnode->prev = NULL;
        deque->size++;
        return;
    }
    deque->head_right->next = newnode;
    newnode->prev = deque->head_right;
    deque->head_right = newnode;
    deque->size++;
}

void pop_back(f_deque* deque){
    if (empty(deque)){
        printf("Can`t pop from back, it`s empty.\n");
        return;
    }
    if(deque->size == 1){
        free(deque->head_right);
        deque->head_left = NULL;
        deque->head_right = NULL;
        deque->size --;
        return;
    }
    node* tmp;
    tmp = deque->head_right->prev;
    deque->head_right = tmp;
    free(tmp->next);
    tmp->next = NULL;
    deque->size--;
}

void push_front(f_deque* deque, double flt){
    node* newnode = (node*)malloc(sizeof(node));
    newnode->prev = NULL;
    newnode->data = flt;
    if(empty(deque)){
        deque->head_left = newnode;
        deque->head_right = newnode;
        newnode->next = NULL;
        deque->size++;
        return;
    }
    deque->head_left->prev = newnode;
    newnode->next = deque->head_left;
    deque->head_left = newnode;
    deque->size++;
}

void pop_front(f_deque* deque){
    if (empty(deque)){
        printf("Can`t pop from front, it`s empty.\n");
        return;
    }
    if(deque->size == 1) {
        free(deque->head_right);
        deque->head_left = NULL;
        deque->head_right = NULL;
        deque->size--;
        return;
    }
    node* tmp;
    tmp = deque->head_left->next;
    deque->head_left = tmp;
    free(tmp->prev);
    tmp->prev = NULL;
    deque->size--;
}

void print_node(node* node){
    printf("%f", node->data);
    if(node->next == NULL){
        return;
    }
    printf(",  ");
    print_node(node->next);
}


void print(f_deque* deque){
    if (empty(deque)){
        printf("Can`t print, it`s empty.\n");
        return;
    }
    print_node(deque->head_left);
    printf("\n");
}

void destroy(f_deque* deque) {
    while(!empty(deque)){
        pop_back(deque);
    }
    free(deque);
}