#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "tarjan.h"
#include "hasse.h"
#include "matrix.h"

#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

int main() {

    // ---------------------------------------------------------------------
    // Configuration Locale et Console
    // ---------------------------------------------------------------------
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "C"); 

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    const char *fichier_mermaid = "graph_output.txt";

    // --- ANCIEN PROJET : MENU DE SELECTION ---
    // Ce menu servait à tester les algorithmes sur plein de petits graphes.
    // Pour le projet de maths, on ne s'intéresse qu'au fichier "matrice projet.txt".
    /*
    int choix = 0;
    do {
        printf("\n== Choix du fichier == \n");
        printf("  1. exemple1.txt\n");
        printf("  2. exemple1_chatGPT_fixed\n");
        printf("  3. exemple1_from_chatGPT.txt\n");
        printf("  4. exemple2.txt\n");
        printf("  5. exemple3.txt\n");
        printf("  6. exemple4_2check.txt\n");
        printf("  7. exemple_hasse1.txt\n");
        printf("  8. exemple_scc1.txt\n");
        printf("  9. exemple_valid_step3.txt\n");
        printf("  10. matrice projet.txt (Le fichier du projet)\n");
        printf("\n  0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
    } while (choix < 0 || choix > 10);

    char *fichier_graphe = NULL;
    switch (choix) {
        case 1: fichier_graphe = "../data/exemple1.txt"; break;
        // ... (autres cas)
        case 10: fichier_graphe = "matrice projet.txt"; break;
        case 0: return 0;
    }
    */

    // --- PROJET MATHS : FICHIER CIBLE UNIQUE ---
    char *fichier_graphe = "../data/matrice_projet.txt";

    printf("\n=== PROJET PROBABILITES : CHAINE DE MARKOV A 27 ETATS ===\n");
    printf("Fichier cible : %s\n", fichier_graphe);

    // ---------------------------------------------------------------------
    // PARTIE 1 : Chargement
    // ---------------------------------------------------------------------

    ListeAdjacence *graphe = readGraph(fichier_graphe);
    if (graphe == NULL) {
        printf("Erreur : Impossible de lire le fichier '%s'. Verifiez qu'il est dans le dossier.\n", fichier_graphe);
        return 1;
    }

    // --- ANCIEN PROJET : AFFICHAGE BRUT DU GRAPHE ---
    // Inutile d'afficher la liste d'adjacence brute pour 27 sommets, cela pollue la console.
    /*
    afficherListeAdjacence(graphe);
    */

    // Utile pour vérifier que la somme des probas fait bien 1 (Propriété stochastique)
    verifierGrapheMarkov(graphe);

    // Génère le fichier pour la visualisation globale (peut servir pour l'intro du rapport)
    genererFichierMermaid(graphe, fichier_mermaid);

    // ---------------------------------------------------------------------
    // PROJET MATHS - QUESTION 1 A 7 : Exploration Numérique
    // ---------------------------------------------------------------------
    // Calcul de l'évolution des probabilités P(n) = P(0) * P^n

    printf("\n=== EXPLORATION NUMERIQUE (Questions 1-7) ===\n");
    printf("Note : Par defaut, simulation pour la Q1 (Depart Etat 2).\n");
    printf("Pour Q2..Q7, modifiez l'initialisation de 'Pi' dans le code ci-dessous.\n");

    t_matrix P = graphe_vers_matrice(graphe);

    // Initialisation vecteur ligne (1 x 27)
    t_matrix Pi = creer_matrice(1, 27);

    // --- CONFIGURATION DEPART (A modifier selon la question) ---
    // Q1 : Départ Etat 2 (Index 1)
    Pi.data[0][1] = 1.0;

    // Exemple pour Q2 (Uniforme sur {2, 5, 12, 21, 25}) - A décommenter si besoin
    /*
    Pi.data[0][1] = 0.2; // Etat 2
    Pi.data[0][4] = 0.2; // Etat 5
    Pi.data[0][11] = 0.2; // Etat 12
    Pi.data[0][20] = 0.2; // Etat 21
    Pi.data[0][24] = 0.2; // Etat 25
    */

    int steps_to_show[] = {1, 2, 10, 50};
    int next_idx = 0;

    printf("Simulation en cours...\n");

    for (int k = 1; k <= 50; k++) {
        t_matrix Pi_next = multiplier_matrices(Pi, P);

        liberer_matrice(Pi);
        Pi = Pi_next;

        // Affichage aux étapes demandées (1, 2, 10, 50)
        if (next_idx < 4 && k == steps_to_show[next_idx]) {
            printf("\n--- Etape n=%d ---\n", k);
            for (int j = 0; j < 27; j++) {
                // On n'affiche que les probas > 0.001 pour la lisibilité
                if (Pi.data[0][j] > 0.001) {
                    printf("Etat %d : %.4f  ", j + 1, Pi.data[0][j]);
                }
            }
            printf("\n");
            next_idx++;
        }
    }
    liberer_matrice(Pi);
    liberer_matrice(P);

    // ---------------------------------------------------------------------
    // PROJET MATHS - QUESTION 8 : Analyse Structurelle (Tarjan & Hasse)
    // ---------------------------------------------------------------------
    // C'est ici qu'on réutilise la puissance de l'ancien projet Algo
    // pour identifier les classes communicantes et le graphe réduit.

    printf("\n=== ANALYSE STRUCTURELLE (Question 8 : Tarjan & Hasse) ===\n");

    // Algo de Tarjan : Trouve les Composantes Fortement Connexes (Classes)
    t_partition partition = algorithmeDeTarjan(graphe);
    afficherPartition(&partition); // Affiche C1 = {...}, C2 = {...}

    // Diagramme de Hasse : Montre les liens entre les classes (Réduction transitive)
    t_link_array reseau_hasse = creer_reseau_hasse(graphe, &partition);
    afficher_hasse(&reseau_hasse); // Donne le code Mermaid pour le graphe réduit

    // Affiche si les classes sont Transitoires ou Finales (Persistantes)
    afficher_caracteristiques(&partition, &reseau_hasse);

    // ---------------------------------------------------------------------
    // PROJET MATHS - QUESTION 10 : Calcul des Distributions Stationnaires
    // ---------------------------------------------------------------------

    printf("\n=== DISTRIBUTIONS STATIONNAIRES (Question 10) ===\n");

    t_matrix M_globale = graphe_vers_matrice(graphe);

    // Tableau pour repérer les classes transitoires
    int *est_transitoire = (int*)calloc(partition.nb_classes, sizeof(int));
    for (int k = 0; k < reseau_hasse.log_size; k++) {
        est_transitoire[reseau_hasse.links[k].from] = 1;
    }

    // Boucle sur chaque classe pour analyser son comportement limite
    for (int i = 0; i < partition.nb_classes; i++) {
        printf("\n>> Analyse Classe C%d : ", i + 1);

        if (est_transitoire[i]) {
            printf("CLASSE TRANSITOIRE.\n");
            printf("   (La probabilité de présence tend vers 0 à l'infini)\n");
        }
        else {
            printf("CLASSE FINALE (Persistante).\n");

            // On extrait la sous-matrice correspondant à cette classe
            t_matrix M_sub = subMatrix(M_globale, partition, i);

            // On cherche la limite de M_sub^n par multiplications successives
            t_matrix M_prev = creer_matrice(M_sub.lignes, M_sub.cols);
            copier_matrice(&M_prev, M_sub);
            t_matrix M_curr;

            float diff = 1.0;
            float epsilon = 0.00001;
            int iter = 0;
            int max_iter = 2000;
            int convergence_atteinte = 0;

            while (iter < max_iter) {
                M_curr = multiplier_matrices(M_prev, M_sub);
                diff = difference_matrices(M_curr, M_prev);

                liberer_matrice(M_prev);
                M_prev = M_curr;
                iter++;

                if (diff < epsilon) {
                    convergence_atteinte = 1;
                    break;
                }
            }

            if (convergence_atteinte) {
                printf("   -> Convergence atteinte (Distribution Stationnaire unique).\n");
                printf("   -> Vecteur Stationnaire (une ligne de la matrice limite) :\n   ");
                // On affiche la première ligne car elles sont toutes identiques à la limite
                for(int c=0; c < M_prev.cols; c++) {
                    printf("%.4f ", M_prev.data[0][c]);
                }
                printf("\n");
            }
            else {
                // Si ça ne converge pas, c'est souvent dû à une périodicité
                printf("   -> Pas de convergence simple.\n");
                printf("   -> Test de Periodicite...\n");
                int periode = getPeriod(M_sub);

                if (periode > 1) {
                    printf("   -> CLASSE PERIODIQUE (d = %d).\n", periode);
                    printf("   -> Pas de limite unique, le systeme cycle.\n");
                } else {
                    printf("   -> Convergence tres lente ou probleme numerique.\n");
                }
            }
            liberer_matrice(M_sub);
            liberer_matrice(M_prev);
        }
    }

    // Nettoyage final
    free(est_transitoire);
    liberer_matrice(M_globale);

    // Libération mémoire structures (simplifié)
    if(partition.classes) {
        for(int i=0; i<partition.nb_classes; i++) free(partition.classes[i].sommets);
        free(partition.classes);
    }
    if(reseau_hasse.links) free(reseau_hasse.links);

    return 0;
}
