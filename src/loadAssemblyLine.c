#include <stdio.h>
#include <stdlib.h>
#include "header.h"

FILE *loadFile(void)
{
    FILE *fp;
    char filepath[50] = "../data/subject/ligneAssemblage";

    do {
        // printf("Choose data filepath:\n");
        // scanf("%s", filepath);
        fp = fopen(filepath, "r");
    } while (fp == NULL);

    return fp;
}

void getAllOpe(assemblyLine_t *line, FILE *fp)
{
    fscanf(fp, "%d%lf", &line->nbOpe, &line->cycleTime);
    line->ope = malloc(sizeof(ope_t *) * (line->nbOpe + 1));
    handleMalloc(line->ope);
    line->ope[line->nbOpe] = NULL;

    for (int i = 0; i < line->nbOpe; i++) {
        line->ope[i] = malloc(sizeof(ope_t));
        handleMalloc(line->ope[i]);
        fscanf(fp, "%lf", &line->ope[i]->time);
        line->ope[i]->id = i + 1;
    }
}

void getAllUnassociable(assemblyLine_t *line, FILE *fp)
{
    int nbUnassociable;
    int opeA, opeB;

    line->unassociable = malloc(sizeof(bool *) * line->nbOpe);
    handleMalloc(line->unassociable);
    for (int i = 0; i < line->nbOpe; i++) {
        line->unassociable[i] = malloc(sizeof(bool) * line->nbOpe);
        handleMalloc(line->unassociable[i]);
        for (int j = 0; j < line->nbOpe; j++)
            line->unassociable[i][j] = false;
    }
    
    fscanf(fp, "%d", &nbUnassociable);

    for (int i = 0; i < nbUnassociable; i++) {
        fscanf(fp, "%d%d", &opeA, &opeB);
        line->unassociable[opeA - 1][opeB - 1] = true;
        line->unassociable[opeB - 1][opeA - 1] = true;
    }

}

void createWorkStations(assemblyLine_t *line)
{
    line->workStation = malloc(sizeof(workStation_t *) * (line->nbOpe + 1));
    handleMalloc(line->workStation);
    line->workStation[line->nbOpe] = NULL;
    for (int i = 0; i < line->nbOpe; i++) {
        line->workStation[i] = malloc(sizeof(workStation_t));
        handleMalloc(line->workStation[i]);
        line->workStation[i]->ope = NULL;
        line->workStation[i]->time = 0.00;
    }
}

assemblyLine_t *loadAssemblyLine(void) // Fonction à aménager -> nouveau sujet
{
    assemblyLine_t *line = malloc(sizeof(assemblyLine_t));
    FILE *fp = loadFile(); //

    handleMalloc(line);
    getAllOpe(line, fp); //
    getAllUnassociable(line, fp); //
    createWorkStations(line);
    line->graph = loadGraph();
    
    fclose(fp);

    return line;
}