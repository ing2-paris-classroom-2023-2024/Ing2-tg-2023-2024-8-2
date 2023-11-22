#include <stdlib.h>
#include "header.h"

int isAssociable2(bool **unassociable, workStation_t *workStation, ope_t *ope, int *idSorted, int nbOpeSorted)
{
    maillon_t *tmpMaillon = workStation->ope;

    while (tmpMaillon != NULL) {
        if (unassociable[ope->id - 1][tmpMaillon->ope->id - 1])
            return 0;
        tmpMaillon = tmpMaillon->next;
    }

    for (int i = 0; i < nbOpeSorted; i++)
        if (idSorted[i] == ope->id)
            return 0;

    return 1;
}

double sortByCycleTime(assemblyLine_t *line, bool isSortByAssociable) // Formulation du candidat le plus grand
{
    int currWS = 0, nbStation = 0, nbOpeSorted = 0;
    int *idSorted = malloc(sizeof(int) * line->nbOpe);
    double totalTime = 0.00;
    ope_t **allOpe = malloc(sizeof(ope_t *) * (line->nbOpe + 1));
    bool changeWS;

    handleMalloc(idSorted);
    handleMalloc(allOpe);
    allOpe[line->nbOpe] = NULL;
    for (int i = 0; i < line->nbOpe; i++)
        allOpe[i] = line->ope[i];
    quickSort(allOpe, 0, line->nbOpe - 1);

    while (nbOpeSorted != line->nbOpe) {
        changeWS = true;
        for (int i = 0; i < line->nbOpe; i++) {
            if (!isSortByAssociable || (isSortByAssociable && isAssociable2(line->unassociable, line->workStation[currWS], allOpe[i], idSorted, nbOpeSorted))) {
                if (line->workStation[currWS]->time + allOpe[i]->time > line->cycleTime)
                    currWS++;
                line->workStation[currWS]->ope = addMaillon(line->workStation[currWS]->ope, allOpe[i]);
                line->workStation[currWS]->time += allOpe[i]->time;
                idSorted[nbOpeSorted] = allOpe[i]->id;
                changeWS = false;
                nbOpeSorted++;
            }
        }
        if (isSortByAssociable && changeWS)
            currWS++;
    }
    line->workStation[currWS + 1] = NULL;
    free(idSorted);
    free(allOpe);

    for (int i = 0; line->workStation[i] != NULL; i++) {
        nbStation++;
        totalTime += line->workStation[i]->time;
    }
    
    return getLoss(line, nbStation, totalTime);
}