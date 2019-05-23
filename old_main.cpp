#include <iostream>
#include <cstring>
#include "Stack.h"

using std::cout;
using std::cin;

bool is_cycle(const char *const matrix, const int d);

bool is_diagonal_zeros(const char *const matrix, const int d);

bool is_complete(const char *const matrix, const int d);

int count_max_degree(const char *const matrix, const int d);

int count_degree_of_row(const char *const matrix, const int row, const int d);

int get_zero_columns(const char *const matrix, const int d, const int *const max_degree_rows, const int rows_size,
                     int *zero_columns);

int
get_rows_with_max_degree(const char *const matrix, const int d, int *const rows_with_max_degree, const int max_degree);

int get_matching_rows(const char *const matrix, const int d, const int *zero_columns, const int zero_columns_size,
                      int *const matching_rows);

void free_arrays(int *arrays[], const int size);

int separate_subgraph(char *const matrix, const int d, char **subgraph);

int get_separated_order(const int d, int *const order, const int *const matching_rows, const int matching_rows_size);

void copy_column(const char *const src, char *const dst, const int which_to_copy, const int where_to_copy, const int d);

void shuffle_matrix(char *const matrix, const int d, const int *const order, const int order_size, char *new_matrix);

void copy_left_upper_corner_of_matrix(const char *const matrix, const int matrix_size, char *const upper_corner,
                                      const int upper_corner_size);

bool connected_graph(const char *const matrix, const int n);

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n = 0;
    cin >> n;
    while (n > 0)
    {

        long d = 0;
        cin >> d;

        char *matrix = (char *) calloc((d * d + 1), sizeof *matrix);
        if (matrix)
        {
            cin >> matrix;

            char *subgraph = nullptr;
            int subgraph_size = separate_subgraph(matrix, d, &subgraph);
            if (connected_graph(matrix, d))
            {
                if (((d % 2 == 1) && is_cycle(matrix, d)) || is_complete(matrix, d))
                {
                    cout << "True\n";
                }
                else { cout << "False\n"; }
            }
            else
            {
                if (subgraph_size > 0 && (((subgraph_size % 2 == 1) && is_cycle(subgraph, subgraph_size)) ||
                                          is_complete(subgraph, subgraph_size)))
                {
                    cout << "True\n";
                }
                else
                {
                    cout << "False\n";
                }
                free(subgraph);

                free(matrix);
            }
        }
        --n;
    }
    return 0;
}

bool connected_graph(const char *const matrix, const int n)
{
    bool *visited = (bool *) calloc(sizeof *visited, n);
    Stack stack;
    int vertex_count = 0;
    stack.push(0);
    visited[0] = true;
    while (stack.empty() == false)
    {
        int current = stack.pop();
        vertex_count++;
        for (int neighbour = 0; neighbour < n; ++neighbour)
        {
            if (matrix[current * n + neighbour] == '1' && !visited[neighbour])
            {
                visited[neighbour] = true;
                stack.push(neighbour);
            }
        }
    }
    free(visited);
    return vertex_count == n;
}

bool is_cycle(const char *const matrix, const int d)
{
    bool cycle_flag = matrix[d - 1] == '1';
    cycle_flag = true;
    if (cycle_flag)
    {
        for (int y = 0; y < d; ++y)
        {
            int cnt = 0;
            for (int x = 0; x < d; ++x)
            {
                if (matrix[y * d + x] == '1')
                {
                    ++cnt;
                }
            }
            cycle_flag = cnt == 2;
            if (!cycle_flag) { break; }
        }
    }
    return cycle_flag;
}

bool is_complete(const char *const matrix, const int d)
{
    int sum = 0;
    for (int i = 0; i < d * d; ++i)
    {
        sum += matrix[i] == '1' ? 1 : 0;
    }
    return sum == d * (d - 1) && is_diagonal_zeros(matrix, d);
}

bool is_diagonal_zeros(const char *const matrix, const int d)
{
    for (int i = 0; i < d; ++i)
    {
        if (matrix[i * (d + 1)] == '1') { return false; }
    }
    return true;
}


// if graph may be not consistent function sorts the matrix in the way that a graph with higher degree will be placed
// in left upper corner of matrix and returns number of vertices of this graph
int separate_subgraph(char *const matrix, const int d, char **subgraph)
{
    int *index_arrays[] = {nullptr, // 1) rows with max degree
                           nullptr, // 2) zero columns
                           nullptr, // 3) matching rows
                           nullptr // 4) order of rows with separated subgraphs
    };
    int index_arrays_size = sizeof(index_arrays) / sizeof(index_arrays[0]);
    enum arrays_index
    {
        MAX_DEGREE_ROWS, ZERO_COLUMNS, MATCHING_ROWS, SEPARATED_ORDER
    };
    // 1) count max degree of the big graph
    int max_degree = count_max_degree(matrix, d);

    // 2) choose rows with max_degree
    index_arrays[MAX_DEGREE_ROWS] = (int *) calloc(sizeof *index_arrays[MAX_DEGREE_ROWS], d);
    if (index_arrays[MAX_DEGREE_ROWS] == nullptr) { free_arrays(index_arrays, index_arrays_size); }
    int rows_with_max_degree_size = get_rows_with_max_degree(matrix, d, index_arrays[MAX_DEGREE_ROWS], max_degree);
    if (rows_with_max_degree_size <= 0)
    {
        free_arrays(index_arrays, index_arrays_size);
        return false;
    }

    // 3) get indexes of columns with zeros from selected rows
    index_arrays[ZERO_COLUMNS] = (int *) calloc(sizeof *index_arrays[ZERO_COLUMNS], d);
    if (index_arrays[ZERO_COLUMNS] == nullptr)
    {
        free_arrays(index_arrays, index_arrays_size);
        return false;
    }
    int zero_columns_size = get_zero_columns(matrix, d, index_arrays[MAX_DEGREE_ROWS], rows_with_max_degree_size,
                                             index_arrays[ZERO_COLUMNS]);
    if (zero_columns_size <= 0)
    {
        free_arrays(index_arrays, index_arrays_size);
        return false;
    }

    // 4) get all rows that match empty columns
    index_arrays[MATCHING_ROWS] = (int *) calloc(sizeof *index_arrays[MATCHING_ROWS], d);
    if (index_arrays[MATCHING_ROWS] == nullptr)
    {
        free_arrays(index_arrays, index_arrays_size);
        return false;
    }
    int matching_rows_size = get_matching_rows(matrix, d, index_arrays[ZERO_COLUMNS], zero_columns_size,
                                               index_arrays[MATCHING_ROWS]);
    if (matching_rows_size <= 0)
    {
        free_arrays(index_arrays, index_arrays_size);
        return false;
    }

    // 5) get order of rows and columns
    index_arrays[SEPARATED_ORDER] = (int *) calloc(sizeof *index_arrays[SEPARATED_ORDER], d);
    if (index_arrays[SEPARATED_ORDER] == nullptr)
    {
        free_arrays(index_arrays, index_arrays_size);

    }
    // TODO append remaning rows after matching rows
    int separated_order_size = get_separated_order(d, index_arrays[SEPARATED_ORDER], index_arrays[MATCHING_ROWS],
                                                   matching_rows_size);


    char *sorted_matrix = (char *) calloc(sizeof *sorted_matrix, d * d);
    if (sorted_matrix)
    {
        shuffle_matrix(matrix, d, index_arrays[SEPARATED_ORDER], separated_order_size, sorted_matrix);

        if (*subgraph) { free(*subgraph); }
        *subgraph = (char *) calloc(sizeof **subgraph, matching_rows_size * matching_rows_size);
        if (*subgraph)
        {
            copy_left_upper_corner_of_matrix(sorted_matrix, d, *subgraph, matching_rows_size);
        }
        free(sorted_matrix);
    }


    free_arrays(index_arrays, index_arrays_size);
    return matching_rows_size;

}

void copy_left_upper_corner_of_matrix(const char *const matrix, const int matrix_size, char *const upper_corner,
                                      const int upper_corner_size)
{
    for (int y = 0; y < upper_corner_size; ++y)
    {
        memcpy(upper_corner + y * upper_corner_size, matrix + y * matrix_size,
               upper_corner_size * sizeof *upper_corner);
    }
}

void shuffle_matrix(char *const matrix, const int d, const int *const order, const int order_size, char *new_matrix)
{
    for (int i = 0; i < d; ++i)
    {
        memcpy(new_matrix + i * d, matrix + order[i] * d, d * sizeof *matrix);
    }
    char *tmp_matrix = (char *) calloc(sizeof *tmp_matrix, d * d);
    memcpy(tmp_matrix, new_matrix, d * d * sizeof *tmp_matrix);
    for (int i = 0; i < d; ++i)
    {
        copy_column(tmp_matrix, new_matrix, order[i], i, d);
    }
    free(tmp_matrix);
}

void copy_column(const char *const src, char *const dst, const int which_to_copy, const int where_to_copy, const int d)
{
    for (int i = 0; i < d; ++i)
    {
        dst[i * d + where_to_copy] = src[i * d + which_to_copy];
    }
}

int get_separated_order(const int d, int *const order, const int *const matching_rows, const int matching_rows_size)
{

    for (int i = 0; i < matching_rows_size; ++i)
    {
        order[i] = matching_rows[i];
    }
    int k = matching_rows_size;
    for (int i = 0; i < d; ++i)
    {
        bool not_appended = true;
        for (int j = 0; j < d; ++j)
        {
            if (order[j] == i)
            {
                not_appended = false;
                break;
            }
        }
        if (not_appended)
        {
            order[k] = i;
            ++k;
        }
    }
    return k;
}

void free_arrays(int *arrays[], const int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (arrays[i] != nullptr)
        {
            free(arrays[i]);
            arrays[i] = nullptr;
        }
    }
}

int get_matching_rows(const char *const matrix, const int d, const int *const zero_columns, const int zero_columns_size,
                      int *const matching_rows)
{
    int size = 0;
    for (int y = 0; y < d; ++y)
    {
        bool match = true;
        for (int i = 0; i < zero_columns_size; ++i)
        {
            if (matrix[y * d + zero_columns[i]] == '1')
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            bool row_of_zeros = true;
            for (int i = 0; i < d; ++i)
            {
                if (matrix[y * d + i] == '1')
                {
                    row_of_zeros = false;
                    break;
                }
            }
            if (!row_of_zeros)
            {
                matching_rows[size] = y;
                ++size;
            }
        }
    }
    return size;
}

int
get_rows_with_max_degree(const char *const matrix, const int d, int *const rows_with_max_degree, const int max_degree)
{
    int size = 0;
    for (int i = 0; i < d; ++i)
    {
        int degree = count_degree_of_row(matrix, i, d);
        if (degree == max_degree)
        {
            rows_with_max_degree[size] = i;
            ++size;
        }
    }
    return size;
}

int get_zero_columns(const char *const matrix, const int d, const int *const max_degree_rows, const int rows_size,
                     int *columns)
{
    int k = 0;
    for (int x = 0; x < d; ++x)
    {
        bool zero_column = true;
        for (int i = 0; i < rows_size; ++i)
        {
            if (matrix[max_degree_rows[i] * d + x] == '1')
            {
                zero_column = false;
                break;
            }
        }
        if (zero_column)
        {
            columns[k] = x;
            ++k;
        }
    }
    return k;
}

int count_max_degree(const char *const matrix, const int d)
{
    int max = 0;
    for (int y = 0; y < d; ++y)
    {
        int current = 0;
        for (int x = 0; x < d; ++x)
        {
            current += matrix[y * d + x] == '1' ? 1 : 0;
        }
        if (current > max) { max = current; }
    }
    return max;
}

int count_degree_of_row(const char *const matrix, const int row, const int d)
{
    int degree = 0;
    for (int i = 0; i < d; ++i)
    {
        degree += matrix[row * d + i] == '1' ? 1 : 0;
    }
    return degree;
}
