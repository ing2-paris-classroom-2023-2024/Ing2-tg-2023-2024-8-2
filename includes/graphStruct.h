#ifndef GRAPHSTRUCT_H
    #define GRAPHSTRUCT_H

typedef struct arc_s {
    struct arc_s *next;
    struct sommet_s *sommet;
    struct sommet_s *origin;
} arc_t;

typedef struct sommet_s {
    int id;
    int deg;
    short couleur;
    int pred;
    int dist;
    arc_t *arc;
} sommet_t;

typedef struct graphe_s {
    int taille;
    int ordre;
    sommet_t **sommets;
} graphe_t;

#endif /*GRAPHSTRUCT_H*/