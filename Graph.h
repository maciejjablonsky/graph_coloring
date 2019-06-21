//
// Created by maciek on 22.05.19.
//

#ifndef PROJECT_3_GRAPH_H
#define PROJECT_3_GRAPH_H

#include <cstdio>
#include <iostream>
class Graph
{
private:
    size_t vertices_number;
    size_t max_degree;
    char *adjacency_matrix;

    bool neighbours(size_t current, size_t neighbour) const;

    bool zeros_on_diagonal(void) const;

    // Graph *create_subgraph_from_visited(size_t *visited_numbers, size_t visited_size);

    void set_vertices_number(size_t n);

    bool is_subgraph_complete(size_t *sub_vertices, size_t n);

    bool is_subgraph_cycle(size_t *sub_vertices, size_t n);

    size_t subgraph_max_degree(size_t *vertices, size_t n);
public:

    Graph();
    ~Graph(void);
    bool load_graph_from_input_stream();
    size_t get_vertices_number(void) const;
    bool connected_graph(void) const;
    bool is_cycle(void) const;
    bool is_complete(void) const;
    size_t get_subgraphs_number(void);
    size_t get_max_degree(void);
    size_t how_many_neighbours(size_t vertex);

    size_t max_chromatic_number_from_subgraphs();

};


#endif //PROJECT_3_GRAPH_H
