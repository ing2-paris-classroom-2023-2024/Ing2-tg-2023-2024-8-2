#include <stdlib.h>
#include <stdio.h>
#include "header.h"

ope_t *getOpeById(ope_t **ope, int id) // Retourne un pointeur d'une opération à partir d'un id
{
    int index = getIndexById(ope, id);

    return ope[index];
}

int isAssociable(maillon_t *file, assemblyLine_t *line, ope_t *currOpe, bool isSortByTime, int currWS)
{ // Retourne 0 si 2 opération ne sont pas associable et 1 sinon
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

maillon_t *getSommetsWithNoDeg(assemblyLine_t *line, int *currWS) // Retourne une liste chainée les sommets de degrés 0 
{
    ope_t *tmpOpe;
    maillon_t *file = NULL;
    bool fileIsEmpty = true;

    for (int i = 0; line->graph->sommets[i] != NULL; i++) {
        if (line->graph->sommets[i]->deg == 0) {
            tmpOpe = getOpeById(line->ope, line->graph->sommets[i]->id);
            file = addMaillon(file, tmpOpe);
            fileIsEmpty = false;
        }
    }

    return file;
}

int getNbPred(sommet_t *sommet) // Retourne le nombre de prédécesseur d'un sommet
{
    arc_t *tmp = sommet->listPred;
    int count = 0;

    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }

    return count;
}

int isPred(int id, arc_t *listPred) // Retourne 1 si le sommet est un prédécesseur
{
    arc_t *tmpArc = listPred;

    while (tmpArc != NULL) {
        if (id == tmpArc->sommet->id)
            return 1;
        tmpArc = tmpArc->next;
    }

    return 0;
}

int getExclusion(assemblyLine_t *line, bool isSortByTime) // Pour les 3 contraintes
{
    maillon_t *tmpMaillon = line->workStation[0]->ope;
    sommet_t *currSommet;
    bool associable;
    int currWS = 0, nbPred, currNbPred, maxNbWS = 0;
    maillon_t *currOpeWS;
    double currTime;

    line->workStation[0]->ope = NULL;
    while (tmpMaillon != NULL) {
        currWS = 0;
        currTime = 0.00;
        currSommet = getSommetById(line->graph->sommets, tmpMaillon->ope->id);
        nbPred = getNbPred(currSommet);
        currNbPred = 0;

        while (1) {
            associable = true;
            currTime = tmpMaillon->ope->time + line->workStation[currWS]->time;
            currOpeWS = line->workStation[currWS]->ope;
            while (currOpeWS != NULL) { // Parcours toute la Work Station
                if (line->unassociable[currOpeWS->ope->index][tmpMaillon->ope->index]) // Trouve une opération pas associable
                    associable = false;
                currNbPred += isPred(currOpeWS->ope->id, currSommet->listPred);
                currOpeWS = currOpeWS->next;
            }
            if ((associable && currNbPred == nbPred && !isSortByTime) || (associable && currNbPred == nbPred && isSortByTime && currTime < line->cycleTime)) {
                // Range une opération dans une work station si toutes les opérations precedente sont passées
                line->workStation[currWS]->ope = addMaillon(line->workStation[currWS]->ope, tmpMaillon->ope);
                line->workStation[currWS]->time += tmpMaillon->ope->time;
                break;
            }
            currWS++;
            if (currWS > maxNbWS)
                maxNbWS = currWS;
        }
        tmpMaillon = tmpMaillon->next;
    }

    return maxNbWS;

}

double kahnAlgorithm(assemblyLine_t *line, bool isSortByTime, bool isSortByAssociable) // Algorithme de tri topologique
{
    int currWS = 0, len, nbStation = 0;
    bool isFinish = false, reboot = false;
    double currTime = 0.00, totalTime = 0.00;
    arc_t *tmpArc;
    ope_t *tmpOpe, **tmpArrayOpe;
    sommet_t *tmpSommet;
    maillon_t *file;

    while (!isFinish) {
        isFinish = true;

        file = getSommetsWithNoDeg(line, &currWS); // File contenant les sommet avec des degrés 0
        if (isSortByTime)
            sortList(&file); // Trie la liste dans l'odre décroissant pour le temps de cycle

        while (file != NULL) {
            isFinish = false;
            reboot = false;

            currTime += file->ope->time;
            if (!isSortByAssociable && isSortByTime && currTime >= line->cycleTime) { // Change de Station
                currTime = file->ope->time;
                currWS++;
            }

            if (!reboot) { // Range toutes les opérations de la file dans une station
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
    if (isSortByAssociable) // 3 contraintes
        currWS = getExclusion(line, isSortByTime);
    line->workStation[currWS + 1] = NULL;
    
    getTimeStation(line->workStation); // Obtient le temps de cycle des stations
    for (int i = 0; line->workStation[i] != NULL; i++) {
        nbStation++;
        totalTime += line->workStation[i]->time;
    }
    
    return getLoss(line, nbStation, totalTime); // Retourne la perte d'équilibrage
}