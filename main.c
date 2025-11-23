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
    afficher_caracteristiques(&partition, &reseau_hasse);


    // Code exécution Partie 3
    //
    // printf("\n=== PARTIE 3 : CALCULS MATRICIELS & PROPRIETES ===\n");
    //
    // t_matrix M_globale = graphe_vers_matrice(graphe);
    // //printf("Matrice de transition M (globale) :\n");    //enlever les // pour voir la matrice en 10x10
    // // afficher_matrice(M_globale);
    //
    // int *est_transitoire = (int*)calloc(partition.nb_classes, sizeof(int));
    // for (int k = 0; k < reseau_hasse.log_size; k++) {
    //     est_transitoire[reseau_hasse.links[k].from] = 1;
    // }
    // printf("\n--- Analyse des Distributions Stationnaires par Classe ---\n");
    //
    // // Boucle sur chaque classe de la partition
    // for (int i = 0; i < partition.nb_classes; i++) {
    //     printf("\n>> Classe C%d : ", i + 1);
    //     if (est_transitoire[i]) {
    //         printf("TRANSITOIRE.\n");
    //         printf("   -> La probabilité limite tend vers 0.\n");
    //     }
    //     else {
    //         printf("PERSISTANTE.\n");
    //
    //         //Etape 2
    //         t_matrix M_sub = subMatrix(M_globale, partition, i);
    //         t_matrix M_prev = creer_matrice(M_sub.lignes, M_sub.cols);
    //         copier_matrice(&M_prev, M_sub);
    //         t_matrix M_curr;
    //
    //         float diff = 1.0;
    //         float epsilon = 0.0001; // Seuil de convergence
    //         int iter = 0;
    //         int max_iter = 1000; // Limite pour éviter boucle infinie
    //         int convergence_atteinte = 0;
    //
    //         while (iter < max_iter) {
    //             M_curr = multiplier_matrices(M_prev, M_sub);
    //             diff = difference_matrices(M_curr, M_prev);
    //
    //             liberer_matrice(M_prev);
    //             M_prev = M_curr;
    //             iter++;
    //
    //             if (diff < epsilon) {
    //                 convergence_atteinte = 1;
    //                 break;
    //             }
    //         }
    //         if (convergence_atteinte) {
    //             printf("   -> Convergence atteinte en %d iterations (diff = %f).\n", iter, diff);
    //             printf("   -> Matrice stationnaire de la classe :\n");
    //             afficher_matrice(M_prev);}
    //         else {
    //             //Etape 3
    //             printf("   -> Pas de convergence simple (Diff = %f apres %d iters).\n", diff, max_iter);
    //             printf("   -> Analyse de PERIODICITE (Bonus)...\n");
    //             int periode = getPeriod(M_sub);
    //
    //             if (periode > 1) {
    //                 printf("   -> La classe est PERIODIQUE de periode d = %d.\n", periode);
    //                 printf("   -> Pas de distribution limite unique, mais une distribution cyclique.\n");
    //             } else {
    //                 printf("   -> Convergence complexe ou très lente.\n");}
    //         }
    //         liberer_matrice(M_sub);
    //         liberer_matrice(M_prev);
    //     }
    // }
    // free(est_transitoire);
    // liberer_matrice(M_globale);
    // free(partition.classes);
    // if(reseau_hasse.links)
    //     free(reseau_hasse.links);
    return 0;
}