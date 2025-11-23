#ifndef MATRIX_H
#define MATRIX_H
#include "graph.h"
#include "tarjan.h"

// Structure matrice simple avec des floats
typedef struct {
    int lignes;
    int cols;
    float **data; // Tableau 2D de flottants
} t_matrix;

// Alloue une matrice de taille rows x cols initialisée à 0
t_matrix creer_matrice(int rows, int cols);
//Convertit une liste d'adjacence en matrice de transition
t_matrix graphe_vers_matrice(ListeAdjacence *g);
//Affiche la matrice dans la console
void afficher_matrice(t_matrix m);
//Copie le contenu de src vers dest
void copier_matrice(t_matrix *dest, t_matrix src);
// Multiplie deux matrices (m1 * m2) et retourne le résultat
t_matrix multiplier_matrices(t_matrix m1, t_matrix m2);
//Calcule la différence (norme) entre deux matrices
float difference_matrices(t_matrix m1, t_matrix m2);
void liberer_matrice(t_matrix m);

//Extrait la sous-matrice correspondant uniquement aux sommets d'une classe.
//(Barème Partie 3 - Extraction de sous-matrice)
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);
//Calcule la période d'une classe (Bonus).
// Retourne d (pgcd des longueurs de cycles).
int getPeriod(t_matrix sub_matrix);

#endif