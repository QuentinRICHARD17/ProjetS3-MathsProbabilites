#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "tarjan.h"
#include "hasse.h"
#include "matrix.h"

#include <locale.h> // IA
#ifdef _WIN32       // IA
#include <windows.h>// IA
#endif              // IA

int main() {

    // ---------------------------------------------------------------------
    // Aide de l'IA (Gemini) : Configuration pour que CLion affiche
    // correctement les accents (tels que "é" ou "à") dans la console.
    // Ceci résout le problème des caractères corrompus (ex: g├®n├®r├®).
    // ---------------------------------------------------------------------

    // 1. Définir la locale du C pour interpréter les chaînes UTF-8
    setlocale(LC_ALL, "");

    setlocale(LC_NUMERIC, "C");
    // Cela force le programme à utiliser le POINT (.) pour les décimales
    // sinon fscanf ne lit pas les "0.25" de ton fichier texte.

    // 2. FORCER le terminal Windows à afficher en UTF-8 (page de code 65001)
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif


    // pour remonter d'un dossier (../) pour trouver "data/exemple1.txt"
    const char *fichier_graphe = "../data/exemple_valid_step3.txt";

    //Pour créer le fichier de sortie dans le dossier parent (../)
    const char *fichier_mermaid = "../graph_output.txt";

    printf("=== LECTURE DU GRAPHE ===\n");
    printf("Fichier : %s\n", fichier_graphe);

    // Code exécution Partie 1

    ListeAdjacence *graphe = readGraph(fichier_graphe);
    if (graphe != NULL) {
        afficherListeAdjacence(graphe);
        verifierGrapheMarkov(graphe);
        genererFichierMermaid(graphe, fichier_mermaid);
    }
    else {
        printf("Erreur\n");
        return 1;
    }


    //  Code exécution Partie 2

    printf("\n=== PARTIE 2 : ANALYSE CONNEXITE (TARJAN & HASSE) ===\n");

    //algo de Tarjan
    t_partition partition = algorithmeDeTarjan(graphe);
    afficherPartition(&partition);

    // diagramme de Hasse
    t_link_array reseau_hasse = creer_reseau_hasse(graphe, &partition);
    afficher_hasse(&reseau_hasse);

    // caract (Transitoire/Persistant/Absorbant/Irréductible)
    afficher_caracteristiques(&partition, &reseau_hasse);


    // Code exécution Partie 3

    // printf("\n=== PARTIE 3 : CALCULS MATRICIELS ===\n");
    //
    // //conversion Graphe -> Matrice
    // t_matrix M = graphe_vers_matrice(graphe);
    // printf("Matrice de transition M (initiale) :\n");
    // afficher_matrice(M);
    //
    // // validation M^3
    // t_matrix M2 = multiplier_matrices(M, M); // M^2
    // t_matrix M3 = multiplier_matrices(M2, M); // M^3
    // printf("\nMatrice M^3 (Probabilites apres 3 etapes) :\n");
    // afficher_matrice(M3);
    //
    // //Convergence vers l'état stationnaire
    // printf("\n--- Test de convergence (Distribution stationnaire) ---\n");
    //
    // t_matrix M_prev = creer_matrice(M.lignes, M.cols);
    // copier_matrice(&M_prev, M);
    // t_matrix M_curr;
    //
    // int n = 1;
    // float epsilon = 0.0001;
    // float diff = 1.0;
    // int max_iter = 100; // eviter boucle infinie
    //
    // while (diff > epsilon && n < max_iter) {
    //     M_curr = multiplier_matrices(M_prev, M);
    //     diff = difference_matrices(M_curr, M_prev);
    //
    //     liberer_matrice(M_prev);
    //     M_prev = M_curr;
    //     n++;
    // }
    //
    // if (n >= max_iter) {
    //     printf("Arrêt : Convergence non atteinte apres %d iterations (diff = %f)\n", max_iter, diff);
    // } else {
    //     printf("Convergence atteinte a n = %d (diff = %f)\n", n, diff);
    //     printf("Matrice stationnaire limite :\n");
    //     afficher_matrice(M_prev);
    // }
    //
    // liberer_matrice(M);
    // liberer_matrice(M2);
    // liberer_matrice(M3);
    // liberer_matrice(M_prev);
    //
    // free(partition.classes);
    // free(reseau_hasse.links);


    return 0;
}