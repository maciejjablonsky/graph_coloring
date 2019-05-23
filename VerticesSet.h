//
// Created by maciek on 23.05.19.
//

#ifndef PROJECT_3_VERTICESSET_H
#define PROJECT_3_VERTICESSET_H

#include <cstdio>

class VerticesSet
{
private:
    size_t init_size;
    size_t * set;
    size_t size;
    size_t i;
public:
    VerticesSet(size_t n);
    size_t next_vertex(void);
    void delete_visited_vertices(bool *visited);
    bool * get_empty_visited_array(void);
    void free_visited(bool * visited);
    bool empty(void);
    size_t get_size(void);
    void go_to(size_t vertex);
    size_t count_visited_vertices(bool * visited);
    size_t * get_numbers_of_visited_vertices(bool*visited);
    void free_visited_vertices_numbers(size_t *visited_vertices);
};


#endif //PROJECT_3_VERTICESSET_H
