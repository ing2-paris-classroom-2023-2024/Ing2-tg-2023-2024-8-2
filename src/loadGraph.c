#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "header.h"

void resetDeg(graphe_t *graph) // Re-initialise les degrés des sommets d'un graphe
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

arc_t *addArc(arc_t *arc, sommet_t *origin, sommet_t *nextNode) // Ajoute un arc à un sommet
{
    arc_t *tmpArc = arc;
    
    if (arc == NULL){
        arc = malloc(sizeof(arc_t));
        handleMalloc(arc);
        arc->sommet = nextNode;
        arc->next= NULL;
        arc->origin = origin;
        return arc;
    }

    while (tmpArc->next != NULL)
        tmpArc = tmpArc->next;

    tmpArc->next = malloc(sizeof(arc_t));
    handleMalloc(tmpArc->next);
    tmpArc->next->next = NULL;
    tmpArc->next->sommet = nextNode;
    tmpArc->next->origin = origin;

    return arc;
}

sommet_t *getSommetById(sommet_t **sommets, int id)
{
    for (int i = 0; sommets[i] != NULL; i++)
        if (sommets[i]->id == id)
            return sommets[i];

    printf("Exit Failure: sommet doesn't exist in graph file\n");
    exit(EXIT_FAILURE);
}

void initGraph(graphe_t *graphe, FILE *fp) // Initialise un graphe
{
    int id1, id2;
    sommet_t *currSommet, *nextSommet;

    for (int i = 0; i < graphe->taille; i++) {
        fscanf(fp, "%d%d", &id1, &id2);
        currSommet = getSommetById(graphe->sommets, id1);
        nextSommet = getSommetById(graphe->sommets, id2);
        currSommet->arc = addArc(currSommet->arc, currSommet, nextSommet);
        nextSommet->deg++;
        nextSommet->listPred = addArc(nextSommet->listPred, nextSommet, currSommet);
    }
}

char *getDirectory(void) // Retourne le chemin vers un dossier existant
{
    DIR *d;
    char *filepath = malloc(sizeof(char) * 100);

    handleMalloc(filepath);
    do {
        system("cls");
        displayLogoECE();
        printColor(PURPLE, "\n\nRentrez l'adresse du dossier de votre ligne d'assemblage:\n\n   ");
        fflush(stdin);
        scanf("%s", filepath);
        d = opendir(filepath);
    } while (d == NULL);

    return filepath;
}

graphe_t *loadGraph(assemblyLine_t *line, char *filepath) // Charge un graphe en mémoire
{
    graphe_t *graphe = malloc(sizeof(graphe_t));
    int nbLine = getNbLine(filepath); // Nombre de ligne du fichier
    FILE *fp = fopen(filepath, "r");

    handleMalloc(graphe);
    if (fp == NULL || nbLine == -1) { // Si le fichier n'est pas en norme
        free(graphe);
        return NULL;
    }

    graphe->ordre = line->nbOpe;
    graphe->sommets = malloc(sizeof(sommet_t *) * (graphe->ordre + 1));
    handleMalloc(graphe->sommets);
    graphe->sommets[graphe->ordre] = NULL;

    for (int i = 0; i < graphe->ordre; i++) { // Init
        graphe->sommets[i] = malloc(sizeof(sommet_t));
        handleMalloc(graphe->sommets[i]);
        graphe->sommets[i]->couleur = 0;
        graphe->sommets[i]->deg = 0;
        graphe->sommets[i]->listPred = NULL;
        graphe->sommets[i]->arc = NULL;
        graphe->sommets[i]->dist = MAX_INT;
        graphe->sommets[i]->pred = -1;
        graphe->sommets[i]->id = line->ope[i]->id;
    }
    graphe->taille = nbLine;

    initGraph(graphe, fp);
    fclose(fp);

    return graphe;
}