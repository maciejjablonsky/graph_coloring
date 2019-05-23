//
// Created by maciek on 23.05.19.
//

#ifndef PROJECT_3_GRAPHVECTOR_H
#define PROJECT_3_GRAPHVECTOR_H

#include <cstdio>

class Graph;

typedef struct graph_node graph_node_t;

struct graph_node
{
    Graph *graph;
    graph_node_t *next;
    graph_node_t *prev;
};


class GraphVector
{
private:
    size_t size;
    graph_node_t *head;
    graph_node_t *tail;
    graph_node_t *current;
    size_t current_i;

    void add_node(Graph *const graph);

    void delete_node(graph_node_t *node);

    void go_to_head(void);

    void go_to_tail(void);

    bool go_to_next(void);

    bool go_to_prev(void);

    void free_vector(void);

public:
    GraphVector(void);

    ~GraphVector(void);

    size_t get_size(void) const;

    void push_back(Graph *const graph);

    Graph *element_at(size_t index);

    void remove(size_t index);

    void clear(void);

    bool empty(void);


};


#endif //PROJECT_3_GRAPHVECTOR_H
