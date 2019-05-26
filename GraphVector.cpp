//
// Created by maciek on 23.05.19.
//

#include "GraphVector.h"
#include "Graph.h"
#include "mem.h"

struct graph_node
{
    Graph *graph;
    graph_node_t *next;
    graph_node_t *prev;
};

GraphVector::GraphVector(void): size(0), head(nullptr), tail(nullptr), current(nullptr), current_i(0)
{
}

GraphVector::~GraphVector(void)
{
    free_vector();
}

void GraphVector::push_back(Graph *const graph)
{
    add_node(graph);
    size++;
}

void GraphVector::add_node(Graph *const graph) {
    graph_node_t * new_node = malloc(graph_node_t, 1);
    if (new_node == nullptr) {exit(1);}

    new_node->graph = graph;
    new_node->next = new_node->prev = nullptr;

    if(head == nullptr)
    {
        head->next = tail->prev = head = tail = new_node;
    }
    else
    {
        new_node->prev = tail;
        tail->next = new_node;
    }
    tail = new_node;
}

void GraphVector::delete_node(graph_node_t *node) {
    graph_node_t tmp = *node;
    if(tmp.prev != nullptr)
    {
        node->prev->next = tmp.next;
    }
    if (tmp.next != nullptr)
    {
        node->next->prev = tmp.prev;
    }
    tail = tmp.prev;
    free(node);
}

void GraphVector::go_to_head(void)
{
    current = head;
}

void GraphVector::go_to_tail(void)
{
    current = tail;
}

bool GraphVector::go_to_next(void)
{
    if(current->next)
    {
        current = current->next;
        return true;
    }
    return false;
}

bool GraphVector::go_to_prev(void)
{
    if(current->prev)
    {
        current = current->prev;
        return true;
    }
    return false;
}

Graph *GraphVector::element_at(size_t index)
{
    if (index >= size || index < 0) { return nullptr;}
    go_to_head();
    for(size_t i = 0; i < index; i++)
    {
        go_to_next();
    }
    return current->graph;
}

void GraphVector::free_vector(void)
{
    while(size > 0)
    {
        delete_node(tail);
        size--;
    }
}

size_t GraphVector::get_size(void) const
{
    return size;
}

void GraphVector::remove(size_t index)
{
    if (index >= size || index < 0) {return;}
    go_to_head();
    for(size_t i = 0; i < index; i++)
    {
        go_to_next();
    }
    delete_node(current);
}

void GraphVector::clear(void)
{
    free_vector();
}

bool GraphVector::empty(void)
{
    return size == 0;
}


