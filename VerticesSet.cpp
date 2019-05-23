//
// Created by maciek on 23.05.19.
//

#include "VerticesSet.h"
#include "mem.h"

VerticesSet::VerticesSet(size_t n) : size(n), i(0), init_size(n)
{
    set = malloc(size_t, size);
    for (size_t k = 0; k < size; ++k)
    {
        set[k] = k;
    }
}

size_t VerticesSet::next_vertex(void)
{
    size_t vertex = set[i];
    i = (i + 1) % size;
    return vertex;
}

bool *VerticesSet::get_empty_visited_array(void)
{
    return calloc(bool, init_size);
}

void VerticesSet::delete_visited_vertices(bool *visited)
{
    size_t new_size = size - count_visited_vertices(visited);
    size_t *new_set = malloc(size_t, size);
    go_to(0);
    for (size_t j = 0, k = 0; j < init_size && k < new_size; ++j)
    {
        int v = next_vertex();
        if (!visited[v])
        {
            new_set[k] = v;
            k++;
        }
    }
    free(set);
    set = new_set;
    size = new_size;
    i = 0;
}

size_t VerticesSet::count_visited_vertices(bool *visited)
{
    size_t count = 0;
    for (size_t j = 0; j < init_size; ++j)
    {
        count += visited[j] ? 1 : 0;
    }
    return count;
}

void VerticesSet::free_visited(bool *visited)
{
    free(visited);
}

bool VerticesSet::empty(void)
{
    return size == 0;
}

size_t VerticesSet::get_size(void)
{
    return size;
}

void VerticesSet::go_to(size_t vertex)
{
    i = vertex;
}

size_t *VerticesSet::get_numbers_of_visited_vertices(bool *visited)
{
    size_t visited_size = count_visited_vertices(visited);
    size_t *vertices = malloc(size_t, visited_size);
    if (!vertices) { exit(1); }
    size_t last_i = i;
    go_to(0);
    for (size_t j = 0, k = 0; j < init_size; ++j)
    {
        size_t v = next_vertex();
        if (visited[v])
        {
            vertices[k] = v;
            k++;
        }
        if (k == visited_size) { break; }
    }
    return vertices;
}

void VerticesSet::free_visited_vertices_numbers(size_t *visited_vertices)
{
    free(visited_vertices);
}





