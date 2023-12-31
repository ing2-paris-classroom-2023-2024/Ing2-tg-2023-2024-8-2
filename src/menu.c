#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void getConstraints(bool *isSortByAssociable, bool *isSortByTime, bool *isSortByPred)
{ // Obtention des contraintes choisis par l'utilisateur
    int choice, tmpChoice;
    bool isValidInput;

    do {
        isValidInput = false;
        system("cls");
        displayLogoECE();
        printColor(GREEN, "\n\nChoisissez vos contraintes:\n\n");
        printColor(PURPLE, "   1 - ");
        printf("Contrainte d'Exclusion\n\n");
        printColor(PURPLE, "   2 - ");
        printf("Contrainte de Precedence\n\n");
        printColor(PURPLE, "   3 - ");
        printf("Contrainte de Temps de Cycle\n\nChoix:  ");
        fflush(stdin);
        scanf("%d", &choice);

        while (choice > 0) {
            tmpChoice = choice % 10;
            choice /= 10;

            if (tmpChoice == 1 && !*isSortByAssociable) {
                *isSortByAssociable = true;
                isValidInput = true;
            } else if (tmpChoice == 2 && !*isSortByPred) {
                *isSortByPred = true;
                isValidInput = true;
            } else if (tmpChoice == 3 && !*isSortByTime) {
                *isSortByTime = true;
                isValidInput = true;
            } else {
                isValidInput = false;
                break;
            }

        }
    } while (!isValidInput);
}

int getChoice(bool badPath) // Obtention du choix du menu principale
{
    int choice;

    do {
        system("cls");
        displayLogoECE();
        if (badPath)
            printColor(RED, "\n\nImpossible de charger la ligne d'assemblage");
        printColor(GREEN, "\n\nMenu :\n\n");
        printColor(CYAN, "   1 - ");
        printf("Charger une ligne d'assemblage\n\n");
        printColor(CYAN, "   2 - ");
        printf("Choix des contraintes\n\n");
        printColor(CYAN, "   3 - ");
        printf("Trouver automatiquement les meilleures contraintes\n\n");
        printColor(CYAN, "   4 - ");
        printf("Changer le temps de cycle\n\n");
        printColor(CYAN, "   5 - ");
        printf("Afficher les infos de la ligne d'assemblage\n\n");
        printColor(CYAN, "   6 - ");
        printf("Quitter\n\n");
        printf("Choix:  ");
        fflush(stdin);
        scanf("%d", &choice);
    } while (choice < 1 || choice > 6);
}

void displayConstraints(assemblyLine_t *line,  bool isSortByAssociable, bool isSortByTime, bool isSortByPred)
{ // Execute et affiche les contraintes choisis
    char c;

    if (isSortByAssociable && !isSortByPred && !isSortByTime)
        sortByUnassociable(line);
    else if (isSortByPred)
        kahnAlgorithm(line, isSortByTime, isSortByAssociable);
    else
        sortByCycleTime(line, isSortByAssociable);

    displayWorkStation(line);

    fflush(stdin);
    gets(&c);

    // Reset les work station
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);
}

void getCycleTime(assemblyLine_t *line)
{ // Met à jour le temps de cycle 
    int min, nbPiece;
    char c;
    bool validInput, tooLittleTime = false;

    do {
        system("cls");
        validInput = true;
        displayLogoECE();
        if (tooLittleTime)
            printColor(RED, "\n\nAu moins une des operations a une duree plus longue que le temps de cycle choisi");
        tooLittleTime = false;
        printColor(GREEN, "\n\nCalculer un nouveau temps de cycle :\n\n");
        printColor(CYAN, "   Combien de minute par jour la ligne d'assemblage est-elle active ?\t");
        fflush(stdin);
        scanf("%d", &min);
        if (min <= 0)
            validInput = false;
        if (validInput) {
            printColor(PURPLE, "\n\n   Combien de pieces par jour souhaitez-vous fabriquer ?\t");
            fflush(stdin);
            scanf("%d", &nbPiece);
            if (nbPiece <= 0)
                validInput = false;
            if (validInput)
                line->cycleTime = min / nbPiece;
            for (int i = 0; i < line->nbOpe; i++)
                if (line->cycleTime < line->ope[i]->time) {
                    validInput = false;
                    tooLittleTime = true;
                    break;
                }
        }
    } while (!validInput);

    printf("\nTemps de Cycle calculer:  %.2f\n", line->cycleTime);

    gets(&c);
    gets(&c);
}

void getBestLoss(assemblyLine_t *line)
{ // Trouve les meilleures contraintes automatiquement
    double results[6];
    char c;
    char sentences[6][85] = {
        {"   Exclusion -> Perte d'Equilibrage de \0"},
        {"   Temps de Cycle -> Perte d'Equilibrage de \0"},
        {"   Precedence + Temps de Cycle -> Perte d'Equilibrage de \0"},
        {"   Precedence + Exclusion -> Perte d'Equilibrage de \0"},
        {"   Temps de Cycle + Exclusion -> Perte d'Equilibrage de \0"},
        {"   Precedence + Exclusion + Temps de Cycle -> Perte d'Equilibrage de \0"}
    };

    results[0] = sortByUnassociable(line);
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);
    results[1] = sortByCycleTime(line, false);
    freeWorkStation(line);
    createWorkStations(line);
    results[2] = kahnAlgorithm(line, true, false);
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);
    results[3] = kahnAlgorithm(line, false, true);
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);
    results[4] = sortByCycleTime(line, true);
    freeWorkStation(line);
    createWorkStations(line);
    results[5] = kahnAlgorithm(line, true, true);
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);

    bubbleSort(results, sentences);

    system("cls");
    displayLogoECE();
    printColor(GREEN, "\n\nMeilleures Contraintes:\n\n");
    for (int i = 0; i < 6; i++) {
        if (results[i] == results[0])
            printf("\033[1;36m");
        printf("%d - %s%.2f%%\n\n", i + 1, sentences[i], results[i]);
        printf("\033[0;37m");
    }
    gets(&c);
    gets(&c);
}