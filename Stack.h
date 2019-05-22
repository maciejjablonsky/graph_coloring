//
// Created by maciek on 21.05.19.
//

#ifndef PROJ3_BFS_STACK_H
#define PROJ3_BFS_STACK_H

#include <cstdio>

typedef int data_t;

class Stack
{
private:
    data_t * values;
    size_t size;
    size_t next_value_index;
    void set_size(void);
    data_t get_top_value(void) const;
    void delete_top_value(void);
public:
    Stack(void);

    ~Stack(void);

    void push(data_t value);

    data_t pop(void);

    bool empty(void) const;

};


#endif //PROJ3_BFS_STACK_H
