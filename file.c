1 /************************/
2 /*** Gestion de liste ***/
3 /************************/
4 #include "file.h"
5 
6 
7 /* 
8 * Creation d'un noeud de valeur x et de suivant suiv 
9 * retourne le noeud cree
10 */
11 Pliste creerEltListe(PArbre x, Pliste suiv) {
12 Pliste ptr;
13 if ((ptr = MALLOC(Liste)) == NULL) {
14 fprintf(stderr, "ERREUR ALLOCATION MEMOIRE FILE");
15 exit(1);
16 }
17 *ptr = (Liste){.val = x, .suiv = suiv };
18 return ptr;
19 }
20 
21 /*
22 * Initialisation des tete et queue de la file
23 */
24 void creerFile(SFile *f) {
25 f->tete = NULL;
26 f->queue = NULL;
27 }
28 
29 /*
30 * Teste de file vide
31 */
32 bool fileVide(SFile f) {
33 if (f.tete == NULL) return true;
34 return false;
35 }
36 
37 /*
38 * Enfilement : on insere l'element 
39 * tete et queue sont modifiees donc passees par adresse (PrtListe *)
40 */
41 void enfiler(SFile *f, PArbre x) {
42 Pliste ptr = creerEltListe(x, NULL);
43 if (f->queue) f->queue->suiv = ptr; // queue pointe sur le nouvel element
44 else f->tete = ptr; // si file vide = nouvel element devient la 
tete
45 f->queue = ptr; // nouvel element = nouvelle queue
46 }
47 
48 /*
49 * Defilement : tete et queue sont modifiees donc passees par adresse (PrtListe *)
50 */
51 PArbre defiler(SFile *f) {
52 Pliste ptr = f->tete;
53 PArbre x;
54 if (!ptr) return ARBRENULL; // File vide
55 f->tete = ptr->suiv; // tete passe au suivant
56 if (f->queue == ptr) f->queue = NULL; // File devient NULL tete = queue
57 x = ptr->val; // on recupere la valeur
58 free(ptr); // on libere l'element
59 return x;
60 }