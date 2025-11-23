#ifndef TARJAN_H
#define TARJAN_H

#ifndef __TARJAN_H__
#define __TARJAN_H__

#include "graph.h"

//Structure pour stocker les métadonnées de Tarjan pour un sommet
typedef struct {
    int id;
    int num;
    int num_accessible;
    int dans_pile;
} t_tarjan_vertex;

//Structure représentant une Classe (Composante Fortement Connexe)
typedef struct {
    int *sommets;
    int nb_sommets;
} t_classe;

//Structure stockant toutes les classes trouvées (Partition)
typedef struct {
    t_classe *classes;
    int nb_classes;
} t_partition;

//Exécute l'algorithme de Tarjan pour trouver les CFC.
//* @param graphe Le graphe à analyser.
//* @return Une partition contenant toutes les classes.
t_partition algorithmeDeTarjan(ListeAdjacence *graphe);
//Affiche le contenu de chaque classe dans la console
void afficherPartition(t_partition *partition);
//Fonction récursive interne pour le parcours en profondeur (DFS)
void parcours(int, int *, int *, t_tarjan_vertex *, int *, t_partition *, ListeAdjacence *);
#endif







#endif //TARJAN_H
