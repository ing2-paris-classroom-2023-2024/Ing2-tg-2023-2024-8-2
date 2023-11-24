#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void getConstraints(bool *isSortByAssociable, bool *isSortByTime, bool *isSortByPred)
{
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

int getChoice(void)
{
    int choice;

    do {
        system("cls");
        displayLogoECE();
        printColor(GREEN, "\n\nMenu :\n\n");
        printColor(CYAN, "   1 - ");
        printf("Choix des contraintes\n\n");
        printColor(CYAN, "   2 - ");
        printf("Trouver automatiquement les meilleures contraintes\n\n");
        printColor(CYAN, "   3 - ");
        printf("Changer le temps de cycle\n\n");
        printColor(CYAN, "   4 - ");
        printf("Quitter\n\n");
        printf("Choix:  ");
        fflush(stdin);
        scanf("%d", &choice);
    } while (choice < 1 || choice > 4);
}

void displayConstraints(assemblyLine_t *line,  bool isSortByAssociable, bool isSortByTime, bool isSortByPred)
{
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
{
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
{
    double results[7];
    char c;
    char sentences[7][85] = {
        {"   Exclusion -> Perte d'Equilibrage de \0"},
        {"   Precedence -> Perte d'Equilibrage de \0"},
        {"   Temps de Cycle -> Perte d'Equilibrage de \0"},
        {"   Precedence + Temps de Cycle -> Perte d'Equilibrage de \0"},
        {"   Precedence + Exclusion -> Perte d'Equilibrage de \0"},
        {"   Temps de Cycle + Exclusion -> Perte d'Equilibrage de \0"},
        {"   Precedence + Exclusion + Temps de Cycle -> Perte d'Equilibrage de \0"}
    };

    results[0] = sortByUnassociable(line);
    freeWorkStation(line);
    createWorkStations(line);
    results[1] = kahnAlgorithm(line, false, false);
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);
    results[2] = sortByCycleTime(line, false);
    freeWorkStation(line);
    createWorkStations(line);
    results[3] = kahnAlgorithm(line, true, false);
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);
    results[4] = kahnAlgorithm(line, false, true);
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);
    results[5] = sortByCycleTime(line, true);
    freeWorkStation(line);
    createWorkStations(line);
    results[6] = kahnAlgorithm(line, true, true);
    freeWorkStation(line);
    createWorkStations(line);
    resetDeg(line->graph);

    bubbleSort(results, sentences);

    system("cls");
    displayLogoECE();
    printColor(GREEN, "\n\nMeilleures Contraintes:\n\n");
    for (int i = 0; i < 7; i++) {
        if (results[i] == results[0])
            printf("\033[1;36m");
        printf("%d - %s%.2f\n\n", i + 1, sentences[i], results[i]);
        printf("\033[0;37m");
    }
    gets(&c);
    gets(&c);
}

void menu(assemblyLine_t *line)
{
    int choice;
    bool isSortByAssociable, isSortByTime, isSortByPred;

    while (choice != 4) {
        isSortByAssociable = false;
        isSortByPred = false;
        isSortByTime = false;
        choice = getChoice();

        switch (choice)
        {
        case 1:
            getConstraints(&isSortByAssociable, &isSortByTime, &isSortByPred);
            displayConstraints(line, isSortByAssociable, isSortByTime, isSortByPred);
            break;
        
        case 2:
            getBestLoss(line);
            break;

        case 3:
            getCycleTime(line);
            break;
        
        default:
            break;
        }
    }
}