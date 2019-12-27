#include "f_deque.h"

extern f_deque* create();
extern void push_back(f_deque* deque, double flt);
extern void pop_back(f_deque* deque);
extern void push_front(f_deque* deque, double flt);
extern void pop_front(f_deque* deque);
extern void print(f_deque* deque);
extern void destroy(f_deque* deque);

int main() {
    f_deque* deque = create();
    pop_back(deque);
    pop_front(deque);
    print(deque);
    push_back(deque, 12.34);
    print(deque);
    pop_front(deque);
    push_front(deque, 12.34);
    print(deque);
    push_back(deque, 56.78);
    push_front(deque, 90.12);
    print(deque);
    pop_front(deque);
    pop_back(deque);
    print(deque);
    pop_back(deque);
    print(deque);
    destroy(deque);
    return 0;
}