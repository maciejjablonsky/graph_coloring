//
// Created by maciek on 23.05.19.
//

#ifndef PROJECT_3_MEM_H
#define PROJECT_3_MEM_H

#include <cstdlib>

#define calloc(TYPE, NUMBER_OF_ELEMENTS)            (TYPE *)calloc(sizeof (TYPE), (NUMBER_OF_ELEMENTS))
#define realloc(ADDRESS, TYPE, NUMBER_OF_ELEMENTS)  (TYPE *)realloc(ADDRESS, sizeof* ADDRESS * (NUMBER_OF_ELEMENTS))
#define malloc(TYPE, NUMBER_OF_ELEMENTS)            (TYPE *)malloc(sizeof(TYPE) * (NUMBER_OF_ELEMENTS))

#endif //PROJECT_3_MEM_H
