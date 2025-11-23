#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>
//Structure représentant une arête vers un sommet adjacent.
typedef struct Cellule {
    int sommet_arrivee;
    float probabilite;
    struct Cellule *suivante;
} Cellule;

//Structure représentant la liste des voisins d'un sommet.
typedef struct Liste {
    Cellule *head;
} Liste;

//tructure principale du graphe (Tableau de listes d'adjacence).
typedef struct ListeAdjacence {
    Liste **listes;
    int taille;
} ListeAdjacence;

//Crée une nouvelle cellule (arête)
Cellule* creerCellule(int arrivee, float proba);

//Crée une liste vide
Liste* creerListe();

//Ajoute une arête au début de la liste d'un sommet
void ajouterCellule(Liste *liste, int arrivee, float proba);

//Crée un graphe vide de taille n
void afficherListe(Liste *liste, int sommet_depart);

//Affiche la liste d'un sommet spécifique
ListeAdjacence* creerListeAdjacence(int taille);

//Affiche tout le graphe dans la console
void afficherListeAdjacence(ListeAdjacence *graphe);

//Lit un graphe depuis un fichier texte
ListeAdjacence* readGraph(const char *filename);

//Vérifie si la somme des probabilités sortantes vaut 1
int verifierGrapheMarkov(ListeAdjacence *graphe);

//Génère un fichier texte compatible avec Mermaid pour la visualisation
void genererFichierMermaid(ListeAdjacence *graphe, const char *nomFichier);

#endif