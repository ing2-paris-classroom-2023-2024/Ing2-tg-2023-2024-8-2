#include <stdlib.h>
#include "header.h"

void insertToList(maillon_t **tete, maillon_t *nouveauMaillon)
{
    maillon_t *courant;

    // Cas spécial : insertion en tête
    if (*tete == NULL || (*tete)->ope->time >= nouveauMaillon->ope->time)
    {
        nouveauMaillon->next = *tete;
        *tete = nouveauMaillon;
    }
    else
    {
        // Trouver le maillon après lequel insérer le nouveau maillon
        courant = *tete;
        while (courant->next != NULL && courant->next->ope->time < nouveauMaillon->ope->time)
            courant = courant->next;

        nouveauMaillon->next = courant->next;
        courant->next = nouveauMaillon;
    }
}

void sortList(maillon_t **tete) // Fonction pour trier la liste chaînée
{
    maillon_t *listeTriee = NULL;
    maillon_t *courant = *tete;
    maillon_t *suivant;

    while (courant != NULL)
    {
        suivant = courant->next; // Sauvegarde du maillon suivant

        // Insérer le maillon courant dans la liste triée
        insertToList(&listeTriee, courant);

        courant = suivant; // Passer au maillon suivant dans la liste non triée
    }

    *tete = listeTriee; // Mettre à jour la tête de la liste chaînée
}

int getLen(maillon_t *list) // Retourne la longueur d'une liste chainée
{
    int count = 0;
    maillon_t *tmpMaillon = list;

    while (tmpMaillon != NULL) {
        count++;
        tmpMaillon = tmpMaillon->next;
    }

    return count;
}

maillon_t *removeMaillon(maillon_t *file) // Supprime un maillon d'une liste chainée
{
    maillon_t *tmpMaillon = file;

    file = file->next;
    free(tmpMaillon);

    return file;
}

maillon_t *addMaillon(maillon_t *maillon, ope_t *ope) // Ajoute un maillon à la liste
{
    maillon_t *tmpMaillon;

    if (maillon == NULL) {
        maillon = malloc(sizeof(maillon));
        handleMalloc(maillon);
        maillon->ope = ope;
        maillon->next = NULL;
        
        return maillon;
    }

    tmpMaillon = maillon;
    while (tmpMaillon->next != NULL)
        tmpMaillon = tmpMaillon->next;
    tmpMaillon->next = malloc(sizeof(maillon_t));
    handleMalloc(tmpMaillon->next);
    tmpMaillon->next->ope = ope;
    tmpMaillon->next->next = NULL;

    return maillon;
}