#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "header.h"

char *catPath(char *str1, char *str2) // Assemble 2 chemin d'accès en une seule string
{
    int len = strlen(str1);
    int len2 = strlen(str2);
    char *fullPath = malloc(sizeof(char) * (len + len2 + 1));

    strcpy(fullPath, str1);
    if (str1[len - 1] != '/')
        strcat(fullPath, "/");
    strcat(fullPath, str2);

    return fullPath;
}

int getNbLine(char *filepath) // Retourne le nombre de ligne d'un fichier
{
    struct stat sb;
    ssize_t rd;
    FILE *fp;
    char *buffer;
    int fd = open(filepath, O_RDONLY);
    int count = 0;

    stat(filepath, &sb);
    if (sb.st_size == 0)
        return -1;
    buffer = malloc(sizeof(char) * (sb.st_size + 1));
    handleMalloc(buffer);

    if (fd != -1)
        rd = read(fd, buffer, sb.st_size);
    else {
        free(buffer);
        return -1;
    }
    if (rd < 0) {
        free(buffer);
        return -1;
    } else
        buffer[rd] = '\0';

    for (int i = 0; buffer[i] != '\0'; i++)
        if (buffer[i] == '\n')
            count++;
    count++;

    close(fd);
    free(buffer);

    return count;
}

int getOpe(assemblyLine_t *line, char *filepath) // Remplit les opérations à partir d'un fichier conf
{
    int nbLine = getNbLine(filepath);
    FILE *fp = fopen(filepath, "r");

    if (nbLine == -1 || fp == NULL)
        return -1;

    line->nbOpe = nbLine;
    line->ope = malloc(sizeof(ope_t *) * (line->nbOpe + 1));
    handleMalloc(line->ope);
    line->ope[line->nbOpe] = NULL;

    for (int i = 0; i < line->nbOpe; i++) {
        line->ope[i] = malloc(sizeof(ope_t));
        handleMalloc(line->ope[i]);
        fscanf(fp, "%d%lf", &line->ope[i]->id, &line->ope[i]->time);
        line->ope[i]->index = i;
    }
    fclose(fp);
    
    return 0;
}

int getIndexById(ope_t **ope, int id) // Retourne l'index d'une opération à partir d'un id
{
    for (int i = 0; ope[i] != NULL; i++)
        if (ope[i]->id == id)
            return i;

    printf("Error: operation not found\n");
    exit(EXIT_FAILURE);
}

int getUnassociable(assemblyLine_t *line, char *filepath) // Remplit les exclusions à partir de fichier conf
{
    int nbLine = getNbLine(filepath);
    FILE *fp = fopen(filepath, "r");
    int opeA, opeB;
    int indexA, indexB;

    if (nbLine == -1 || fp == NULL)
        return -1;

    line->unassociable = malloc(sizeof(bool *) * line->nbOpe);
    handleMalloc(line->unassociable);
    for (int i = 0; i < line->nbOpe; i++) {
        line->unassociable[i] = malloc(sizeof(bool) * line->nbOpe);
        handleMalloc(line->unassociable[i]);
        for (int j = 0; j < line->nbOpe; j++)
            line->unassociable[i][j] = false;
    }

    for (int i = 0; i < nbLine; i++) {
        fscanf(fp, "%d%d", &opeA, &opeB);
        indexA = getIndexById(line->ope, opeA);
        indexB = getIndexById(line->ope, opeB);
        line->unassociable[indexA][indexB] = true;
        line->unassociable[indexB][indexA] = true;
    }
    fclose(fp);

    return 0;
}

void createWorkStations(assemblyLine_t *line) // Créer les Work Station
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

assemblyLine_t *loadAssemblyLine(void) // Charge une nouvelle ligne d'assemblage
{
    assemblyLine_t *line = malloc(sizeof(assemblyLine_t));
    FILE *fp;
    char *filepath = getDirectory();
    char *exclusionPath = catPath(filepath, "exclusions.txt");
    char *precedencePath = catPath(filepath, "precedences.txt");
    char *operationPath = catPath(filepath, "operations.txt");
    char *cycleTimePath = catPath(filepath, "temps_cycle.txt");

    handleMalloc(line);
    if ((fp = fopen(cycleTimePath, "r")) == NULL)
        return NULL;
    fscanf(fp, "%lf", &line->cycleTime);
    fclose(fp);
    if (getOpe(line, operationPath) == -1)
        return NULL;
    if (getUnassociable(line, exclusionPath) == -1)
        return NULL;
    createWorkStations(line);
    line->graph = loadGraph(line, precedencePath);
    if (line->graph == NULL)
        return NULL;

    free(filepath);
    free(exclusionPath);
    free(precedencePath);
    free(operationPath);
    free(cycleTimePath);

    return line;
}