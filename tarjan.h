#ifndef TARJAN_H
#define TARJAN_H

#ifndef __TARJAN_H__
#define __TARJAN_H__

#include "graph.h"

typedef struct {
    int id;
    int num;
    int num_accessible;
    int dans_pile;
} t_tarjan_vertex;

typedef struct {
    int *sommets;
    int nb_sommets;
} t_classe;


typedef struct {
    t_classe *classes;
    int nb_classes;
} t_partition;


t_partition algorithmeDeTarjan(ListeAdjacence *graphe);
void afficherPartition(t_partition *partition);
void parcours(int, int *, int *, t_tarjan_vertex *, int *, t_partition *, ListeAdjacence *);
#endif







#endif //TARJAN_H
