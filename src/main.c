#include "header.h"

int main(void)
{
    assemblyLine_t *line = loadAssemblyLine();

    menu(line);

    freeAssemblyLine(line);

    return 0;
}