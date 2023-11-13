#include <stdlib.h>

void handleMalloc(void *ptr)
{
    if (ptr == NULL) {
        printf("Exit Failure: malloc failed\n");
        exit(EXIT_FAILURE);
    }
}