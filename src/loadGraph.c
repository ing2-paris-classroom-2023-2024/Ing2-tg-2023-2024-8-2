#include <stdlib.h>
#include <stdio.h>
#include "header.h"

void resetDeg(graphe_t *graph)
{
    arc_t *tmpArc;

    for (int i = 0; graph->sommets[i] != NULL; i++)
        graph->sommets[i]->deg = 0;

    for (int i = 0; graph->sommets[i] != NULL; i++) {
        tmpArc = graph->sommets[i]->arc;
        while (tmpArc != NULL) {
            tmpArc->sommet->deg++;
            tmpArc = tmpArc->next;
        }
    }
}

arc_t *addArc(sommet_t *origin, sommet_t *nextNode)
{
    arc_t *tmpArc = origin->arc;
    
    nextNode->deg += 1;
    if (origin->arc == NULL){
        origin->arc = malloc(sizeof(arc_t));
        handleMalloc(origin->arc);
        origin->arc->sommet = nextNode;
        origin->arc->next= NULL;
        origin->arc->origin = origin;
        return origin->arc;
    }

    while (tmpArc->next != NULL)
        tmpArc = tmpArc->next;

    tmpArc->next = malloc(sizeof(arc_t));
    handleMalloc(tmpArc->next);
    tmpArc->next->next = NULL;
    tmpArc->next->sommet = nextNode;
    tmpArc->next->origin = origin;

    return origin->arc;
}

sommet_t *getSommetById(sommet_t **sommets, int id)
{
    for (int i = 0; sommets[i] != NULL; i++)
        if (sommets[i]->id == id)
            return sommets[i];

    printf("Exit Failure: sommet doesn't exist in graph file\n");
    exit(EXIT_FAILURE);
}

void initGraph(graphe_t *graphe, FILE *fp)
{
    int id1, id2;
    sommet_t *currSommet, *nextSommet;

    for (int i = 0; i < graphe->ordre; i++) {
        graphe->sommets[i]->arc = NULL;
        graphe->sommets[i]->dist = MAX_INT;
        graphe->sommets[i]->pred = -1;
    }

    for (int i = 0; i < graphe->taille; i++) {
        fscanf(fp, "%d%d", &id1, &id2);
        currSommet = getSommetById(graphe->sommets, id1);
        nextSommet = getSommetById(graphe->sommets, id2);
        currSommet->arc = addArc(currSommet, nextSommet);
    }
}

FILE *getFile(void)
{
    FILE *fp;
    char filepath[50] = "../data/subject/graphe";

    do {
        // printf("Choisir le chemin du fichier graphe:\n");
        // scanf("%s", filepath);
        fp = fopen(filepath, "r");
    } while (fp == NULL);

    return fp;
}

graphe_t *loadGraph(void)
{
    graphe_t *graphe = malloc(sizeof(graphe_t));
    FILE *fp = getFile();

    handleMalloc(graphe);
    fscanf(fp, "%d", &graphe->ordre);
    graphe->sommets = malloc(sizeof(sommet_t *) * (graphe->ordre + 1));
    handleMalloc(graphe->sommets);
    graphe->sommets[graphe->ordre] = NULL;

    for (int i = 0; i < graphe->ordre; i++) {
        graphe->sommets[i] = malloc(sizeof(sommet_t));
        handleMalloc(graphe->sommets[i]);
        graphe->sommets[i]->couleur = 0;
        graphe->sommets[i]->deg = 0;
        fscanf(fp, "%d", &graphe->sommets[i]->id);
    }
    fscanf(fp, "%d", &graphe->taille);

    initGraph(graphe, fp);

    fclose(fp);

    return graphe;
}