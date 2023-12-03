#include <stdlib.h>

void handleMalloc(void *ptr) // Verifie si un pointeur est correctement alloué
{
    if (ptr == NULL) {
        printf("Exit Failure: malloc failed\n");
        exit(EXIT_FAILURE);
    }
}