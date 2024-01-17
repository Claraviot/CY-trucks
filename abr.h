#ifndef ABR_H
#define ABR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arbreBinaire.h"

typedef struct pile {
Element val;
struct pile *suiv;
} Pile;
typedef Pile* PPile;

/*********************************
* Prototypes ABR
*/
PArbre RechercheABR(PArbre , Element );
PArbre InsereABR(PArbre , Element );
PArbre InsereABRIteratif(PArbre , Element );
PArbre SuppMax(PArbre , Element *);
PArbre SuppABR(PArbre , Element );
bool estUnABR(PArbre );
PArbre calculHauteursABR(PArbre );
PArbre calculEquilibresABR(PArbre );
PPile ABtoPile(PArbre , PPile );
bool estUnABR(PArbre );

#endif
