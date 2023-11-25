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
        choice = getChoice(badPath);

        switch (choice)
        {
        case 1:
            if (isAllocated && !badPath)
                freeAssemblyLine(line);
            line = loadAssemblyLine();
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
                getConstraints(&isSortByAssociable, &isSortByTime, &isSortByPred);
                displayConstraints(line, isSortByAssociable, isSortByTime, isSortByPred);
            }
            break;
        
        case 3:
            if (isAllocated)
                getBestLoss(line);
            break;

        case 4:
            if (isAllocated)
                getCycleTime(line);
            break;

        case 5:
            if (isAllocated)
                displayLineInfos(line);
            break;
        
        default:
            break;
        }
    }
    if (isAllocated)
        freeAssemblyLine(line);

    return 0;
}