//
// Created by maciek on 22.05.19.
//

#ifndef PROJECT_3_GRAPH_H
#define PROJECT_3_GRAPH_H

#include <cstdio>
#include <iostream>
#include "GraphVector.h"
class Graph
{
private:
    GraphVector subgraphs;
    size_t vertices_number;
    bool neighbours(size_t current, size_t neighbour) const;
    bool zeros_on_diagonal(void) const;
    Graph * create_subgraph_from_visited(size_t * visited_numbers, size_t visited_size);
    void set_vertices_number(size_t n);
    size_t max_degree;
    char *adjacency_matrix;
public:

    Graph();
    ~Graph(void);
    bool load_graph_from_input_stream();
    size_t get_vertices_number(void) const;
    bool connected_graph(void) const;
    bool is_cycle(void) const;
    bool is_complete(void) const;
    void load_subgraphs(void);
    size_t get_subgraphs_number(void);
    Graph * subgraph(size_t index);
    size_t get_max_degree(void);
    size_t how_many_neighbours(size_t vertex);

};


#endif //PROJECT_3_GRAPH_H
