#ifndef MATRIX_H
#define MATRIX_H
#include "graph.h"
#include "tarjan.h"

typedef struct {
    int lignes;
    int cols;
    float **data; // Tableau 2D de flottants
} t_matrix;

t_matrix creer_matrice(int rows, int cols);
t_matrix graphe_vers_matrice(ListeAdjacence *g);
void afficher_matrice(t_matrix m);
void copier_matrice(t_matrix *dest, t_matrix src);
t_matrix multiplier_matrices(t_matrix m1, t_matrix m2);
float difference_matrices(t_matrix m1, t_matrix m2);
void liberer_matrice(t_matrix m);

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);

#endif