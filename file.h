/************************/
/*** Gestion de liste ***/
/************************/
#ifndef FILE_H 
#define FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arbreBinaire.h"
/*
* Structure de file d'arbre basee sur la liste
*/
typedef struct lst {
PArbre val;
struct lst * suiv;
} Liste;
typedef Liste* Pliste;
typedef struct sfile {
Pliste tete;
Pliste queue;
} SFile;
/*********************************
* Prototypes Liste
*/
Pliste creerEltListe(PArbre, Pliste);
void creerFile(SFile *);
bool fileVide(SFile );
void enfiler(SFile *, PArbre);
PArbre defiler(SFile *);
#endif