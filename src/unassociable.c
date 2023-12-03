#include <stdlib.h>
#include "header.h"

void getTimeStation(workStation_t **workStation)
{
    double time = 0.00;
    maillon_t *tmpMaillon;

    for (int i = 0; workStation[i] != NULL; i++) {
        tmpMaillon = workStation[i]->ope;

        while (tmpMaillon != NULL) {
            time += tmpMaillon->ope->time;
            tmpMaillon = tmpMaillon->next;
        }

        workStation[i]->time = time;
        time = 0.00;
    }
}

void loadWorkStation(assemblyLine_t *line, int **matWS)
{
    bool endLoop = false;
    int currWS = -1;

    while (!endLoop) {
        currWS++;
        endLoop = true;
        for (int i = 0; i < line->nbOpe; i++) {
            if (matWS[i][currWS]) {
                line->workStation[currWS]->ope = addMaillon(line->workStation[currWS]->ope, line->ope[i]);
                endLoop = false;
            }
        }
    }

    line->workStation[currWS] = NULL;
}

double sortByUnassociable(assemblyLine_t *line)
{
    int **matWS = malloc(sizeof(int *) * line->nbOpe);
    bool unassociable = false;
    int currWS = -1, nbStation = 0;
    double totalTime = 0.00;

    handleMalloc(matWS);
    for (int i = 0; i < line->nbOpe; i++) {
        matWS[i] = malloc(sizeof(int) * line->nbOpe);
        handleMalloc(matWS);
        for (int j = 0; j < line->nbOpe; j++)
            matWS[i][j] = 0;
    }

    for (int i = 0; i < line->nbOpe; i++) {
        do {
            currWS++;
            unassociable = false;
            for (int j = 0; j < line->nbOpe; j++)
                if (matWS[j][currWS] && line->unassociable[i][j]) {
                    unassociable = true;
                    break;
                }
        } while (unassociable);

        matWS[i][currWS] = 1;
        currWS = -1;
    }

    loadWorkStation(line, matWS);
    getTimeStation(line->workStation);

    for (int i = 0; i < line->nbOpe; i++)
        free(matWS[i]);
    free(matWS);
    for (int i = 0; line->workStation[i] != NULL; i++) {
        nbStation++;
        totalTime += line->workStation[i]->time;
    }
    
    return getLoss(line, nbStation, totalTime);
}