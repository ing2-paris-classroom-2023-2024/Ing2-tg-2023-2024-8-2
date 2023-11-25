#include <stdlib.h>
#include <stdio.h>
#include "header.h"

ope_t *getOpeById(ope_t **ope, int id)
{
    int index = getIndexById(ope, id);

    return ope[index];
}

maillon_t *removeMaillon(maillon_t *file)
{
    maillon_t *tmpMaillon = file;

    file = file->next;
    free(tmpMaillon);

    return file;
}

int isAssociable(maillon_t *file, assemblyLine_t *line, ope_t *currOpe, bool isSortByTime, int currWS)
{
    maillon_t *tmpMaillon = file;

    while (tmpMaillon != NULL) {
        if (line->unassociable[tmpMaillon->ope->index][currOpe->index])
            return 0;
        tmpMaillon = tmpMaillon->next;
    }

    tmpMaillon = line->workStation[currWS]->ope;
    while (isSortByTime && tmpMaillon != NULL) {
        if (line->unassociable[tmpMaillon->ope->index][currOpe->index])
            return 0;
        tmpMaillon = tmpMaillon->next;
    }

    return 1;
}

maillon_t *getSommetsWithNoDeg(assemblyLine_t *line, bool isSortByAssociable, bool isSortByTime, int *currWS)
{
    ope_t *tmpOpe;
    maillon_t *file = NULL;
    int returnIsAssociable = 0;
    bool fileIsEmpty = true;

    for (int i = 0; line->graph->sommets[i] != NULL; i++) {
        if ((line->graph->sommets[i]->deg == 0 && !isSortByAssociable) || (line->graph->sommets[i]->deg == 0 && isSortByAssociable && (returnIsAssociable = isAssociable(file, line, getOpeById(line->ope, line->graph->sommets[i]->id), isSortByTime, *currWS)))) {
            tmpOpe = getOpeById(line->ope, line->graph->sommets[i]->id);
            file = addMaillon(file, tmpOpe);
            fileIsEmpty = false;
        }
    }

    if (isSortByAssociable && !returnIsAssociable && fileIsEmpty) {
        *currWS += 1;
        for (int i = 0; line->graph->sommets[i] != NULL; i++) {
            if ((line->graph->sommets[i]->deg == 0 && !isSortByAssociable) || (line->graph->sommets[i]->deg == 0 && isSortByAssociable && (returnIsAssociable = isAssociable(file, line, getOpeById(line->ope, line->graph->sommets[i]->id), isSortByTime, *currWS)))) {
                tmpOpe = getOpeById(line->ope, line->graph->sommets[i]->id);
                file = addMaillon(file, tmpOpe);
                fileIsEmpty = true;
            }
        }
    }

    return file;
}

double kahnAlgorithm(assemblyLine_t *line, bool isSortByTime, bool isSortByAssociable) // Algorithme de tri topologique
{
    int currWS = -1, len, nbStation = 0;
    bool isFinish = false, reboot = false;
    double currTime = 0.00, totalTime = 0.00;
    arc_t *tmpArc;
    ope_t *tmpOpe, **tmpArrayOpe;
    sommet_t *tmpSommet;
    maillon_t *file;
    printf("oui\n");

    while (!isFinish) {
        isFinish = true;
        if (!isSortByTime || currWS == -1)
            currWS++;

        file = getSommetsWithNoDeg(line, isSortByAssociable, isSortByTime, &currWS);
        if (isSortByTime)
            sortList(&file);

        while (file != NULL) {
            isFinish = false;
            reboot = false;

            // currTime += file->ope->time;
            if (isSortByTime && currTime + file->ope->time > line->cycleTime) { // Change de Station
                int len = getLen(file);
                file = getSommetsWithNoDeg(line, isSortByAssociable, isSortByTime, &currWS);
                sortList(&file);
                if (len == getLen(file)) {
                    currWS++;
                    currTime = file->ope->time;
                } else if (currTime + file->ope->time > line->cycleTime)
                    reboot = true;
            } else
                currTime += file->ope->time;

            if (!reboot) {
                line->workStation[currWS]->ope = addMaillon(line->workStation[currWS]->ope, file->ope);

                tmpSommet = getSommetById(line->graph->sommets, file->ope->id);
                tmpArc = tmpSommet->arc;
                while (tmpArc != NULL) {
                    tmpArc->sommet->deg -= 1;
                    tmpArc = tmpArc->next;
                }

                tmpSommet->deg = -1;
                file = removeMaillon(file);
            }
        }
    }
    if (!isSortByTime && isSortByAssociable)
        line->workStation[currWS - 1] = NULL;
    if (!isSortByTime || (isSortByAssociable && isSortByTime))
        line->workStation[currWS] = NULL;
    else
        line->workStation[currWS + 1] = NULL;
    getTimeStation(line->workStation);
    for (int i = 0; line->workStation[i] != NULL; i++) {
        nbStation++;
        totalTime += line->workStation[i]->time;
    }
    
    return getLoss(line, nbStation, totalTime);
}