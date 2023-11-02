#ifndef HEADER_H
#define HEADER_H

#include "struct.h"

#define MAX_INT 2147483647

#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"

// handleError
void handleMalloc(void *ptr);

// sortByCycleTime
double sortByCycleTime(assemblyLine_t *line, bool isSortByAssociable);

// cycleTime
double kahnAlgorithm(assemblyLine_t *line, bool isSortByTime, bool isSortByAssociable);

// loadGraph
graphe_t *loadGraph(void);
sommet_t *getSommetById(sommet_t **sommets, int id);
void resetDeg(graphe_t *graph);

// loadAssemblyLine
assemblyLine_t *loadAssemblyLine(void);
void createWorkStations(assemblyLine_t *line);

// linkedList
maillon_t *addMaillon(maillon_t *maillon, ope_t *ope);
void sortList(maillon_t **head);
int getLen(maillon_t *list);

// free
void freeAssemblyLine(assemblyLine_t *line);
void freeWorkStation(assemblyLine_t *line);

// menu
void menu(assemblyLine_t *line);

// display
void displayWorkStation(assemblyLine_t *line);
void printColor(char *color, char *str);
void displayLogoECE(void);
double getLoss(assemblyLine_t *line, int nbStation, double totalTime);

// quickSort
void quickSort(ope_t **ope, int left, int right);

// unassociable
double sortByUnassociable(assemblyLine_t *line);
void getTimeStation(workStation_t **workStation);

#endif /*HEADER_H*/