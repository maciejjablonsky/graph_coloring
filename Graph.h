//
// Created by maciek on 22.05.19.
//

#ifndef PROJECT_3_GRAPH_H
#define PROJECT_3_GRAPH_H

#include <cstdio>
#include <iostream>
#include <fstream>
class Graph
{
private:
    char *adjacency_matrix;
    size_t vertices_number;
    bool neighbours(size_t current, size_t neighbour) const;
    bool zeros_on_diagonal(void) const;
public:
    Graph();
    ~Graph(void);
    bool load_graph_from_input_stream();
    size_t get_vertices_number(void) const;
    bool connected_graph(void) const;
    bool is_cycle(void) const;
    bool is_complete(void) const;
};


#endif //PROJECT_3_GRAPH_H
