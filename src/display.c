#include <stdlib.h>
#include <stdio.h>
#include "header.h"

void displayLogoECE(void)
{
    printf("\033[1;31m");
    printf("\t\t____________________ ___________\n\t\t\\_   _____/\\_   ___ \\_   _____/\n\t\t |    __)_ /    \\  \\/ |    __)_ \n\t\t");
    printf(" |        \\     \\____|         \\\n\t\t/_______  / \\______  /_______  /\n\t\t        \\/         \\/        \\/ \n");
    printf("\033[0;37m");
}

void printColor(char *color, char *str)
{
    printf("%s", color);
    printf("%s", str);
    printf("\033[0;37m");
}

double getLoss(assemblyLine_t *line, int nbStation, double totalTime)
{
    return (1.00 - (totalTime / (nbStation * line->cycleTime))) * 100;
}

void displayWorkStation(assemblyLine_t *line)
{
    maillon_t *tmpMaillon;
    int nbStation = 0;
    double loss, totalTime = 0.00;

    for (int i = 0; line->workStation[i] != NULL; i++) {
        nbStation++;
        totalTime += line->workStation[i]->time;
    }
    loss = getLoss(line, nbStation, totalTime);
    system("cls");
    displayLogoECE();
    printColor(BLUE, "\n\nPerte d'equilibrage: ");
    printf("%.2f%%\n\n", loss);
    printColor(RED, "Nombre de station: ");
    printf("%d\n\n", nbStation);

    for (int i = 0; line->workStation[i] != NULL; i++) {
        tmpMaillon = line->workStation[i]->ope;
        printColor(GREEN, "Station ");
        printf("%d:\n\n", i + 1);
        printColor(CYAN, "     Operations: ");
        while(tmpMaillon != NULL) {
            printf("%d ", tmpMaillon->ope->id);
            tmpMaillon = tmpMaillon->next;
        }
        printColor(CYAN, "\n\n     Temps de cycle: ");
        printf("%.2f sec\n\n", line->workStation[i]->time);
    }
}

void diplayGraph(graphe_t *graph)
{
    arc_t *tmpArc;
    
    for (int i = 0; i < graph->ordre; i++) {
        printf("Sommet %d:   ", graph->sommets[i]->id);
        tmpArc = graph->sommets[i]->arc;
        while (tmpArc != NULL) {
            printf("%d ", tmpArc->sommet->id);
            tmpArc = tmpArc->next;
        }
        printf("\n");
    }
}