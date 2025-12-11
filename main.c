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
    char *fichier_graphe = NULL;
    int mode_projet = 0;
    int choix_fichier = 0;

    // ---------------------------------------------------------------------
    // MENU PRINCIPAL (HIERARCHIQUE)
    // ---------------------------------------------------------------------
    do {//menu de base
        printf("\n===========================================\n");
        printf("           MENU DE SELECTION\n");
        printf("===========================================\n");
        printf("  1. Projet Info (Anciens tests Algorithmiques)\n");
        printf("  2. Projet Proba (Chaines de Markov)\n");
        printf("  0. Quitter\n");
        printf("\nVotre choix : ");
        scanf("%d", &mode_projet);

        if (mode_projet == 0) return 0;

        if (mode_projet == 1) {
            // --- SOUS-MENU : PROJET INFO ---
            printf("\n--- FICHIERS PROJET INFO ---\n");
            printf("  1. exemple1.txt\n");
            printf("  2. exemple1_chatGPT_fixed.txt\n");
            printf("  3. exemple1_from_chatGPT.txt\n");
            printf("  4. exemple2.txt\n");
            printf("  5. exemple3.txt\n");
            printf("  6. exemple4_2check.txt\n");
            printf("  7. exemple_hasse1.txt\n");
            printf("  8. exemple_scc1.txt\n");
            printf("  9. exemple_valid_step3.txt\n");
            printf("  0. Retour\n");
            printf("Votre choix : ");
            scanf("%d", &choix_fichier);

            switch (choix_fichier) {
                case 1: fichier_graphe = "../data/exemple1.txt"; break;
                case 2: fichier_graphe = "../data/exemple1_chatGPT_fixed.txt"; break;
                case 3: fichier_graphe = "../data/exemple1_from_chatGPT.txt"; break;
                case 4: fichier_graphe = "../data/exemple2.txt"; break;
                case 5: fichier_graphe = "../data/exemple3.txt"; break;
                case 6: fichier_graphe = "../data/exemple4_2check.txt"; break;
                case 7: fichier_graphe = "../data/exemple_hasse1.txt"; break;
                case 8: fichier_graphe = "../data/exemple_scc1.txt"; break;
                case 9: fichier_graphe = "../data/exemple_valid_step3.txt"; break;
                case 0: mode_projet = -1; break; // Pour boucler
                default: printf("Choix invalide.\n"); mode_projet = -1; break;
            }

        } else if (mode_projet == 2) {
            // --- SOUS-MENU : PROJET PROBA ---
            printf("\n--- FICHIERS PROJET PROBA ---\n");
            printf("  1. Matrice Projet (27 etats)\n");
            // Tu pourras ajouter d'autres fichiers ici plus tard
            // printf("  2. Autre fichier de test...\n");
            printf("  0. Retour\n");
            printf("Votre choix : ");
            scanf("%d", &choix_fichier);

            switch (choix_fichier) {
                case 1: fichier_graphe = "../data/matrice_projet.txt"; break;
                    // case 2: fichier_graphe = "../data/ton_nouveau_fichier.txt"; break;
                case 0: mode_projet = -1; break;
                default: printf("Choix invalide.\n"); mode_projet = -1; break;
            }
        } else {
            printf("Saisie incorrecte.\n");
        }

    } while (mode_projet < 1 || fichier_graphe == NULL);


    printf("\n>>> Chargement du fichier : %s\n", fichier_graphe);

    // ---------------------------------------------------------------------
    // PARTIE 1 : Chargement
    // ---------------------------------------------------------------------

    ListeAdjacence *graphe = readGraph(fichier_graphe);
    if (graphe == NULL) {
        printf("Erreur : Impossible de lire le fichier '%s'. Verifiez le chemin.\n", fichier_graphe);
        return 1;
    }

    // On verifie si c'est un graphe stochastique (Markov)
    verifierGrapheMarkov(graphe);

    // Generation visuelle
    genererFichierMermaid(graphe, fichier_mermaid);

    // ---------------------------------------------------------------------
    // EXPLORATION NUMERIQUE (Adaptée pour fonctionner avec n'importe quelle taille)
    // ---------------------------------------------------------------------
    // Cette partie n'est pertinente que pour les graphes de Markov,
    // mais on la laisse s'exécuter pour voir le comportement même sur les graphes "Info".

    printf("\n=== EXPLORATION NUMERIQUE ===\n");

    t_matrix P = graphe_vers_matrice(graphe);
    int taille = graphe->taille; // Utilisation de la taille réelle du graphe

    // Initialisation vecteur ligne (1 x taille)
    t_matrix Pi = creer_matrice(1, taille);

    // --- CONFIGURATION DEPART ---
    // On met 1.0 sur l'état 2 (si il existe), sinon sur l'état 1
    if (taille >= 2) {
        Pi.data[0][1] = 1.0; // Départ Etat 2
    } else {
        Pi.data[0][0] = 1.0; // Départ Etat 1 (si graphe minuscule)
    }

    int steps_to_show[] = {1, 2, 10, 50};
    int next_idx = 0;

    printf("Simulation de l'evolution (P^n) en cours...\n");

    for (int k = 1; k <= 50; k++) {
        t_matrix Pi_next = multiplier_matrices(Pi, P);

        liberer_matrice(Pi);
        Pi = Pi_next;

        // Affichage aux étapes demandées
        if (next_idx < 4 && k == steps_to_show[next_idx]) {
            printf("\n--- Etape n=%d ---\n", k);
            for (int j = 0; j < taille; j++) {
                if (Pi.data[0][j] > 0.001) { // Seuil d'affichage
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
    // ANALYSE STRUCTURELLE (Tarjan & Hasse)
    // ---------------------------------------------------------------------

    printf("\n=== ANALYSE STRUCTURELLE (Tarjan & Hasse) ===\n");

    t_partition partition = algorithmeDeTarjan(graphe);
    afficherPartition(&partition);

    t_link_array reseau_hasse = creer_reseau_hasse(graphe, &partition);
    afficher_hasse(&reseau_hasse);

    afficher_caracteristiques(&partition, &reseau_hasse);

    // ---------------------------------------------------------------------
    // DISTRIBUTIONS STATIONNAIRES
    // ---------------------------------------------------------------------

    printf("\n=== DISTRIBUTIONS STATIONNAIRES (Analyse Limite) ===\n");

    t_matrix M_globale = graphe_vers_matrice(graphe);

    int *est_transitoire = (int*)calloc(partition.nb_classes, sizeof(int));
    for (int k = 0; k < reseau_hasse.log_size; k++) {
        est_transitoire[reseau_hasse.links[k].from] = 1;
    }

    for (int i = 0; i < partition.nb_classes; i++) {
        printf("\n>> Analyse Classe C%d : ", i + 1);

        if (est_transitoire[i]) {
            printf("CLASSE TRANSITOIRE.\n");
        }
        else {
            printf("CLASSE FINALE (Persistante).\n");

            t_matrix M_sub = subMatrix(M_globale, partition, i);
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
                printf("   -> Convergence atteinte.\n");
                printf("   -> Vecteur Stationnaire :\n   ");
                for(int c=0; c < M_prev.cols; c++) {
                    printf("%.4f ", M_prev.data[0][c]);
                }
                printf("\n");
            }
            else {
                printf("   -> Pas de convergence simple.\n");
                int periode = getPeriod(M_sub);
                if (periode > 1) {
                    printf("   -> CLASSE PERIODIQUE (d = %d).\n", periode);
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

    if(partition.classes) {
        for(int i=0; i<partition.nb_classes; i++) free(partition.classes[i].sommets);
        free(partition.classes);
    }
    if(reseau_hasse.links) free(reseau_hasse.links);

    // Petite pause pour Windows pour ne pas fermer la fenetre direct
    printf("\nAppuyez sur Entree pour quitter...");
    getchar(); getchar();

    return 0;
}