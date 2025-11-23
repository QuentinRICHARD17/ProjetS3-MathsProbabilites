#ifndef __HASSE_H__
#define __HASSE_H__

#include "tarjan.h"
#include "graph.h"

typedef struct {
    int from;
    int to;
} t_link;

typedef struct {
    t_link *links;
    int log_size;
} t_link_array;


void removeTransitiveLinks(t_link_array *p_link_array);


t_link_array creer_reseau_hasse(ListeAdjacence *g, t_partition *p);
void afficher_hasse(t_link_array *r);
void afficher_caracteristiques(t_partition *p, t_link_array *r);

void removeTransitiveLinks(t_link_array *p_link_array);

#endif