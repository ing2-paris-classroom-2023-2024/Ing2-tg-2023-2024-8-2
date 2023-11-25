#ifndef HEADER_H
    #define HEADER_H

#include <stdio.h>
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
graphe_t *loadGraph(assemblyLine_t *line, char *filepath);
sommet_t *getSommetById(sommet_t **sommets, int id);
void resetDeg(graphe_t *graph);

// loadAssemblyLine
assemblyLine_t *loadAssemblyLine(void);
void createWorkStations(assemblyLine_t *line);
char *catPath(char *str1, char *str2);
int getNbLine(char *filepath);

int getIndexById(ope_t **ope, int id);

// linkedList
maillon_t *addMaillon(maillon_t *maillon, ope_t *ope);
void sortList(maillon_t **head);
int getLen(maillon_t *list);

// free
void freeAssemblyLine(assemblyLine_t *line);
void freeWorkStation(assemblyLine_t *line);
void freeGraph(graphe_t *graphe);

// menu
void getConstraints(bool *isSortByAssociable, bool *isSortByTime, bool *isSortByPred);
int getChoice(bool badPath);
void displayConstraints(assemblyLine_t *line,  bool isSortByAssociable, bool isSortByTime, bool isSortByPred);
void getCycleTime(assemblyLine_t *line);
void getBestLoss(assemblyLine_t *line);

// display
void displayWorkStation(assemblyLine_t *line);
void printColor(char *color, char *str);
void displayLogoECE(void);
double getLoss(assemblyLine_t *line, int nbStation, double totalTime);
void displayLineInfos(assemblyLine_t *line);

// quickSort
void quickSort(ope_t **ope, int left, int right);
void bubbleSort(double arr[], char str[7][85]);

// unassociable
double sortByUnassociable(assemblyLine_t *line);
void getTimeStation(workStation_t **workStation);

char *getDirectory(void);

#endif /*HEADER_H*/