//
// Created by maciek on 21.05.19.
//

#include "Stack.h"
#include <cstring>

Stack::Stack() : size(2), next_value_index(0)
{
    values = new data_t[size];
}

Stack::~Stack()
{
    delete[] values;
}

void Stack::set_size()
{
    if (next_value_index + 1 == size)
    {
        data_t *new_values = new data_t[size * 2];
        if (new_values)
        {
            memcpy(new_values, values, size * sizeof(data_t));
            delete[]values;
            values = new_values;
            size *= 2;
        }
    }
}

void Stack::push(data_t value)
{
    set_size();
    values[next_value_index] = value;
    next_value_index++;
}

data_t Stack::pop(void)
{
    data_t top_value = get_top_value();
    delete_top_value();
    return top_value;
}

data_t Stack::get_top_value() const
{
    return values[next_value_index - 1];
}

void Stack::delete_top_value()
{
    if (next_value_index > 0)
    {
        next_value_index--;
    }

}

bool Stack::empty(void) const
{
    return next_value_index == 0;
}