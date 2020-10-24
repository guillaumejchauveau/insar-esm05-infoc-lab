#include<stdio.h>
#include<stddef.h>

#ifndef MYALLOC_H
#define MYALLOC_H

/*
 * paramétrage du module
 */
/* commenter cette ligne pour ne plus afficher les messages de debug */
/* #define DEBUG */

/* commenter cette ligne pour utiliser les fonctions d'allocation standard du C */
#define ALLOC_PERSO

#ifdef ALLOC_PERSO
#define malloc(size) Mon_malloc(size)
#define free(ptr) Mon_free(ptr)
#define calloc(n,size) Mon_calloc(n,size)
#define realloc(ptr,new_size) Mon_realloc(ptr,new_size)
#endif

/*
 * taille du tas en octet
 */
#define MAX_MEMORY 1000000

/*
 * nature occupée ou libre d'une zone mémoire
 */
typedef enum {LIBRE, OCCUPE} Statut_memoire;

/*
 * structure de descripteur de bloc mémoire :
 */
typedef struct descript{
 size_t size;           /* sa taille en octets           */
 Statut_memoire statut; /* il est libre ou occupé        */
 struct descript *suiv; /* chaînage vers le bloc suivant */
} Descript_mem;

/* si diagnostique ptr NULL on arrête (exit) ou en continue */
typedef enum {ARRET, CONTINUER} Diag_ptr;
/*
 * prototypes des fonctions exportable (utilisables ailleur) de ce module
 */

/* test si un pointeur est valide et si NULL réagit selon le paramètre Diag_ptr (ARRET ou CONTINUER) */
void test_pointeur(void * t, Diag_ptr d);

/* affiche à l'écran la cartographie mémoire (info détaillée sur chacune des zones) */
void stat_memoire_dyn();

/* mise en oeuvre du malloc() sur la structure de tableau de caractères */
void *Mon_malloc(size_t nb_octets);

void Mon_free(void* ptr);
void *Mon_calloc(size_t count, size_t size);
void *Mon_realloc(void* ptr, size_t size);
#endif /* MYALLOC_H */
