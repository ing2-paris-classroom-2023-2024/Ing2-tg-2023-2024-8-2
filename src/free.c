#include <stdlib.h>
#include "header.h"

void freeWorkStation(assemblyLine_t *line) // Libère les stations de travail
{
    maillon_t *tmpMaillon, *tmpMaillon2;
    bool endToFree = false;

    for (int i = 0; i < line->nbOpe; i++) {
        if (line->workStation[i] == NULL)
            endToFree = true;
        if (!endToFree) {
            tmpMaillon = line->workStation[i]->ope;
            while (tmpMaillon != NULL) {
                tmpMaillon2 = tmpMaillon->next;
                free(tmpMaillon);
                tmpMaillon = tmpMaillon2;
            }
        }
        free(line->workStation[i]);
    }
    free(line->workStation);
}

void freeGraph(graphe_t *graphe) // Libère un graphe
{
    arc_t *tmpArc, *tmpArc2;

    for (int i = 0; i < graphe->ordre; i++) {
        tmpArc = graphe->sommets[i]->arc;
        while (tmpArc != NULL)  {
            tmpArc2 = tmpArc->next;
            free(tmpArc);
            tmpArc = tmpArc2;
        }
        tmpArc = graphe->sommets[i]->listPred;
        while (tmpArc != NULL) {
            tmpArc2 = tmpArc->next;
            free(tmpArc);
            tmpArc = tmpArc2;
        }
        free(graphe->sommets[i]);
    }
    free(graphe->sommets);
    free(graphe);
}

void freeAssemblyLine(assemblyLine_t *line) // Libère une ligne d'assemblage
{
    maillon_t *tmpMaillon, *tmpMaillon2;
    bool endToFree = false;

    for (int i = 0; i < line->nbOpe; i++) {
        if (line->workStation[i] == NULL)
            endToFree = true;
        if (!endToFree) {
            tmpMaillon = line->workStation[i]->ope;
            while (tmpMaillon != NULL) {
                tmpMaillon2 = tmpMaillon->next;
                free(tmpMaillon);
                tmpMaillon = tmpMaillon2;
            }
        }
        free(line->workStation[i]);
        free(line->ope[i]);
        free(line->unassociable[i]);
    }
    free(line->workStation);
    free(line->ope);
    freeGraph(line->graph);
    free(line->unassociable);
    
    free(line);
}