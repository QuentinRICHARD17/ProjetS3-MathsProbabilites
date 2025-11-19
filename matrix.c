#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Pour fabs()
#include "matrix.h"

t_matrix creer_matrice(int lignes, int cols) {
    t_matrix m;
    m.lignes = lignes;
    m.cols = cols;

    m.data = (float**)malloc(lignes * sizeof(float*));
    int i, j;
    for (i = 0; i < lignes; i++) {
        m.data[i] = (float*)malloc(cols * sizeof(float));
        // Init à 0
        for (j = 0; j < cols; j++) {
            m.data[i][j] = 0.0;
        }
    }
    return m;
}

t_matrix graphe_vers_matrice(ListeAdjacence *g) {
    int n = g->taille;
    t_matrix m = creer_matrice(n, n);
    int i;
    for (i = 1; i <= n; i++) {
        Cellule *curr = g->listes[i]->head;
        while (curr != NULL) {
            int col = curr->sommet_arrivee - 1;
            int row = i - 1;
            
            m.data[row][col] = curr->probabilite;
            
            curr = curr->suivante;
        }
    }
    return m;
}

void afficher_matrice(t_matrix m) {
    int i, j;
    printf("\n--- Matrice (%d x %d) ---\n", m.lignes, m.cols);
    for (i = 0; i < m.lignes; i++) {
        for (j = 0; j < m.cols; j++) {
            printf("%.2f\t", m.data[i][j]);
        }
        printf("\n");
    }
}
void copier_matrice(t_matrix *dest, t_matrix src) {
    int i, j;
    for (i = 0; i < src.lignes; i++) {
        for (j = 0; j < src.cols; j++) {
            dest->data[i][j] = src.data[i][j];
        }
    }
}
t_matrix multiplier_matrices(t_matrix m1, t_matrix m2) {
    // On suppose que dimensions ok (m1.cols == m2.lignes)
    t_matrix res = creer_matrice(m1.lignes, m2.cols);
    
    int i, j, k;
    for (i = 0; i < m1.lignes; i++) {
        for (j = 0; j < m2.cols; j++) {
            float somme = 0;
            for (k = 0; k < m1.cols; k++) {
                somme += m1.data[i][k] * m2.data[k][j];
            }
            res.data[i][j] = somme;
        }
    }
    return res;
}
float difference_matrices(t_matrix m1, t_matrix m2) {
    float diff = 0;
    int i, j;
    
    for (i = 0; i < m1.lignes; i++) {
        for (j = 0; j < m1.cols; j++) {
            diff += fabs(m1.data[i][j] - m2.data[i][j]);
        }
    }
    return diff;
}

void liberer_matrice(t_matrix m) {
    int i;
    for (i = 0; i < m.lignes; i++) {
        free(m.data[i]);
    }
    free(m.data);
}

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
    t_matrix res;
    int nb = part.classes[compo_index].nb_sommets;

    res = creer_matrice(nb, nb);

    int i, j;
    for (i = 0; i < nb; i++) {
        for (j = 0; j < nb; j++) {
            // On récupère l'ID réel du sommet dans le graphe
            int id_reelle_ligne = part.classes[compo_index].sommets[i];
            int id_reelle_col = part.classes[compo_index].sommets[j];

            // On va chercher la valeur dans la grande matrice originale
            res.data[i][j] = matrix.data[id_reelle_ligne - 1][id_reelle_col - 1];
        }
    }

    return res;
}