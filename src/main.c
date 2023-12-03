#include "header.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    assemblyLine_t *line;
    int choice;
    bool isSortByAssociable, isSortByTime, isSortByPred;
    bool isAllocated = false, badPath = false;

    while (choice != 6) {
        isSortByAssociable = false;
        isSortByPred = false;
        isSortByTime = false;
        choice = getChoice(badPath); // Retourne le choix de l'utilisateur

        switch (choice)
        {
        case 1:
            if (isAllocated && !badPath)
                freeAssemblyLine(line); // Libère une ligne d'assemblage
            line = loadAssemblyLine(); // Charge une nouvelle ligne d'assemblage
            if (line == NULL) {
                badPath = true;
                isAllocated = false;
            } else {
                badPath = false;
                isAllocated = true;
            }
            break;
            
        case 2:
            if (isAllocated) {
                getConstraints(&isSortByAssociable, &isSortByTime, &isSortByPred); // Retourne les contraintes choisis par l'utilisateur
                displayConstraints(line, isSortByAssociable, isSortByTime, isSortByPred); // Applique les contraintes et les affiche
            }
            break;
        
        case 3:
            if (isAllocated)
                getBestLoss(line); // Test et affiche toutes les contraintes en trouvant les meilleures
            break;

        case 4:
            if (isAllocated)
                getCycleTime(line); // Change le temps de cycle de la ligne d'asseblage
            break;

        case 5:
            if (isAllocated)
                displayLineInfos(line); // Affiche les infos d'une ligne d'assemblage
            break;
        
        default:
            break;
        }
    }
    if (isAllocated)
        freeAssemblyLine(line); // Libère la ligne d'assemblage

    return 0;
}