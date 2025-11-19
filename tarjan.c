#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarjan.h"


void parcours(int sommet_actuel, int *pile, int *ptr_pile, t_tarjan_vertex *infos,
              int *compteur, t_partition *partition, ListeAdjacence *graphe) {

    int v;
    Cellule *voisin;
    infos[sommet_actuel].num = *compteur;


    infos[sommet_actuel].num_accessible = *compteur;
    (*compteur)++;
    pile[*ptr_pile] = sommet_actuel;
    (*ptr_pile)++;
    infos[sommet_actuel].dans_pile = 1;
    voisin = graphe->listes[sommet_actuel]->head;

    while (voisin) {
        v = voisin->sommet_arrivee;

        if (infos[v].num == -1) {
            parcours(v, pile, ptr_pile, infos, compteur, partition, graphe);

            if (infos[v].num_accessible < infos[sommet_actuel].num_accessible) {
                infos[sommet_actuel].num_accessible = infos[v].num_accessible;
            }
        }
        else {
            if (infos[v].dans_pile) {
                if (infos[v].num < infos[sommet_actuel].num_accessible) {
                    infos[sommet_actuel].num_accessible = infos[v].num;
                }
            }
        }
        voisin = voisin->suivante;
    }

    if (infos[sommet_actuel].num_accessible == infos[sommet_actuel].num) {
        int *temp;
        int nb = 0;
        int val;
        int k;

        temp = (int*)malloc(graphe->taille * sizeof(int));
        if (!temp) return;
        do {
            (*ptr_pile)--;
            val = pile[*ptr_pile];
            infos[val].dans_pile = 0;
            temp[nb] = val;
            nb++;
        } while (val != sommet_actuel);

        partition->nb_classes++;
        partition->classes = (t_classe*)realloc(partition->classes, partition->nb_classes * sizeof(t_classe));

        int idx = partition->nb_classes - 1;
        partition->classes[idx].nb_sommets = nb;
        partition->classes[idx].sommets = (int*)malloc(nb * sizeof(int));

        for(k = 0; k < nb; k++) {
            partition->classes[idx].sommets[k] = temp[k];
        }

        free(temp);
    }
}



t_partition algorithmeDeTarjan(ListeAdjacence *graphe) {
    t_partition resultat;
    int n;
    int i;

    resultat.classes = NULL;
    resultat.nb_classes = 0;
    n = graphe->taille;

    t_tarjan_vertex *infos = malloc((n + 1) * sizeof(t_tarjan_vertex));
    int *pile = malloc((n + 1) * sizeof(int));


    if (!infos || !pile) {
        printf("Erreur allocation\n");
        return resultat;
    }

    int ptr_pile = 0;
    int compteur = 0;

    // Boucle d'initialisation
    for (i = 1; i <= n; i++) {
        infos[i].id = i;
        infos[i].num = -1;
        infos[i].num_accessible = -1;
        infos[i].dans_pile = 0;
    }

    // Lancement du parcours
    for (i = 1; i <= n; i++) {
        if (infos[i].num == -1) {
            parcours(i, pile, &ptr_pile, infos, &compteur, &resultat, graphe);
        }
    }
    free(infos);
    free(pile);
    return resultat;
}



void afficherPartition(t_partition *partition) {

    printf("\n--- Resultat Partition --\n");
    printf("Nombre de classes : %d\n", partition->nb_classes);

    for (int i = 0; i < partition->nb_classes; i++) {
        printf("C%d : { ", i + 1);
        for (int j = 0; j < partition->classes[i].nb_sommets; j++) {
            printf("%d", partition->classes[i].sommets[j]);
            if (j < partition->classes[i].nb_sommets - 1) {
                printf(", ");
            }
        }
        printf(" }\n");
    }
}