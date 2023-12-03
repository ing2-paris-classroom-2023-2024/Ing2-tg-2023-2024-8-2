#include <stdlib.h>
#include <string.h>
#include "header.h"

void bubbleSort(double arr[], char str[6][85]) // Tri à bulle
{
    bool sorted = false;
    double tmpVal;
    char tmpStr[85];

    while (!sorted) {
        sorted = true;
        for (int i = 0; i < 5; i++) {
            if (arr[i] > arr[i + 1]) {
                sorted = false;
                tmpVal = arr[i];
                strcpy(tmpStr, str[i]);
                arr[i] = arr[i + 1];
                arr[i + 1] = tmpVal;
                strcpy(str[i], str[i + 1]);
                strcpy(str[i + 1], tmpStr);
            }
        }
    }
}

void quickSort(ope_t **ope, int left, int right) // Tri rapide
{
    int i = left, j = right;
    ope_t *temp;
    double pivot = ope[(left + right) / 2]->time;
 
    while (i <= j) {
        while (ope[i]->time < pivot)
            i++;
        while (ope[j]->time > pivot)
            j--;

        if (i <= j) {
            temp = ope[i];
            ope[i] = ope[j];
            ope[j] = temp;

            i++;
            j--;
        }
    }
 
    if (left < j)
        quickSort(ope, left, j);
    if (i < right)
        quickSort(ope, i, right);
}