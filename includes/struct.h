#ifndef STRUCT_H
#define STRUCT_H

#include <stdbool.h>
#include "graphStruct.h"

typedef struct ope_s
{
    int id;
    double time;
} ope_t;

typedef struct maillon_s
{
    ope_t *ope;
    struct maillon_s *next;
} maillon_t;

typedef struct workStation_s
{
    maillon_t *ope;
    double time;
} workStation_t;

typedef struct assemblyLine_s
{
    ope_t **ope;
    workStation_t **workStation;
    graphe_t *graph;
    bool **unassociable;
    double cycleTime;
    int nbOpe;
} assemblyLine_t;

#endif /*STRUCT_H*/