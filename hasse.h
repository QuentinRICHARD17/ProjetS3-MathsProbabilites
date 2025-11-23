#ifndef __HASSE_H__
#define __HASSE_H__

#include "tarjan.h"
#include "graph.h"
//Structure représentant un lien orienté entre deux classes
typedef struct {
    int from;
    int to;
} t_link;
//Tableau dynamique de liens pour le graphe de Hasse
typedef struct {
    t_link *links;
    int log_size;
} t_link_array;


void removeTransitiveLinks(t_link_array *p_link_array);


 //Construit le graphe de Hasse à partir du graphe original et de sa partition.
 //@param g Le graphe original.
 //@param p La partition (les classes).
 //@return Un tableau de liens représentant Hasse (avec réduction transitive).

t_link_array creer_reseau_hasse(ListeAdjacence *g, t_partition *p);
//Affiche les liens du diagramme de Hasse
void afficher_hasse(t_link_array *r);
//Affiche les propriétés (Transitoire, Persistante, Absorbant)
void afficher_caracteristiques(t_partition *p, t_link_array *r);
//Supprime les liens transitifs (Option du barème) : si A->B et B->C, on supprime A->C
void removeTransitiveLinks(t_link_array *p_link_array);

#endif