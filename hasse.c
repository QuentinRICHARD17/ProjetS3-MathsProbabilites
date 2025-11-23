#include <malloc.h>
#include "hasse.h"
#include <stdio.h>
#include <stdlib.h>


t_link_array creer_reseau_hasse(ListeAdjacence *g, t_partition *p) {
    t_link_array res;
    res.links = NULL;
    res.log_size = 0;

    int n = g->taille;
    int *map = (int*)malloc((n + 1) * sizeof(int));

    if (!map) return res;

    int i, j;
    for (i = 0; i < p->nb_classes; i++) {
        for (j = 0; j < p->classes[i].nb_sommets; j++) {
            map[p->classes[i].sommets[j]] = i;
        }
    }

    for (i = 1; i <= n; i++) {
        Cellule *c = g->listes[i]->head;
        while (c) {
            int v = c->sommet_arrivee;
            int c1 = map[i];
            int c2 = map[v];

            if (c1 != c2) {
                int k;
                int existe = 0;
                for (k = 0; k < res.log_size; k++) {
                    if (res.links[k].from == c1 && res.links[k].to == c2) {
                        existe = 1;
                    }
                }

                if (!existe) {
                    res.log_size++;
                    res.links = (t_link*)realloc(res.links, res.log_size * sizeof(t_link));
                    res.links[res.log_size - 1].from = c1;
                    res.links[res.log_size - 1].to = c2;
                }
            }
            c = c->suivante;
        }
    }
    free(map);

    removeTransitiveLinks(&res);
    return res;
}

void afficher_hasse(t_link_array *r) {
    printf("\n--- Diagramme de Hasse (Format Mermaid) ---\n");
    printf("Copiez-collez ceci dans https://mermaid.live/ :\n\n");
    printf("flowchart TD\n"); // Début du graphe Mermaid

    if (r->log_size == 0) {
        printf("    %% Aucun lien inter-classes\n");
    }

    int i;
    for (i = 0; i < r->log_size; i++) {
        // Format : C1 --> C2
        printf("    C%d --> C%d\n", r->links[i].from + 1, r->links[i].to + 1);
    }
    printf("\n-------------------------------------------\n");
}

void afficher_caracteristiques(t_partition *p, t_link_array *r) {
    printf("\n--- Caracteristiques ---\n");

    if (p->nb_classes == 1) printf("Graphe irreductible.\n");
    else printf("Graphe non irreductible.\n");

    int *transitoire = (int*)malloc(p->nb_classes * sizeof(int));
    int i, k;

    for (i = 0; i < p->nb_classes; i++) {
        transitoire[i] = 0;
    }


    for (k = 0; k < r->log_size; k++) {
        transitoire[r->links[k].from] = 1;
    }

    for (i = 0; i < p->nb_classes; i++) {
        printf("C%d : ", i + 1);
        if (transitoire[i]) {
            printf("TRANSITOIRE\n");
        } else {
            printf("PERSISTANTE");
            if (p->classes[i].nb_sommets == 1) {
                printf(" (Etat ABSORBANT : %d)", p->classes[i].sommets[0]);
            }
            printf("\n");
        }
    }
    free(transitoire);
}

void removeTransitiveLinks(t_link_array *p_link_array) {
    int i, j, k;
    for (i = 0; i < p_link_array->log_size; i++) {
        for (j = 0; j < p_link_array->log_size; j++) {
            if (p_link_array->links[i].to == p_link_array->links[j].from) {
                int a = p_link_array->links[i].from;
                int c = p_link_array->links[j].to;
                for (k = 0; k < p_link_array->log_size; k++) {
                    if (p_link_array->links[k].from == a && p_link_array->links[k].to == c) {
                        p_link_array->links[k] = p_link_array->links[p_link_array->log_size - 1];
                        p_link_array->log_size--;
                        k--;
                    }
                }
            }
        }
    }
}