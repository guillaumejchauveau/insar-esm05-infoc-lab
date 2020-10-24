#include <stdio.h>
#include<stdlib.h>
#include "myalloc.h"
#include "test.h"

/*
 * fonction de test de l'allocation et de la libération mémoire
 * avec affichage a chaque opération de la cartographie du tas
 * à la fin des libérations on doit avoir le bon même nombre d'octets
 * libres qu'au début
 */
static void test_malloc_1(){
    int * p, *r, *k;
    char *q, *w;
    /* test malloc et free */
    p=(int *)malloc(100*sizeof(int));
    test_pointeur((void *) p, CONTINUER);
    stat_memoire_dyn();

    q=(char *)malloc(250*sizeof(char));
    test_pointeur((void *) q, CONTINUER);
    stat_memoire_dyn();

    r=(int *)malloc(1000*sizeof(int));
    test_pointeur((void *) r, CONTINUER);
    stat_memoire_dyn();

    free(p);
    stat_memoire_dyn();

    w=(char *)malloc(700);
    test_pointeur((void *) w, CONTINUER);
    stat_memoire_dyn();

    free(r);
    stat_memoire_dyn();

    k=(int *)malloc(1000*sizeof(int));
    test_pointeur((void *) k, CONTINUER);
    stat_memoire_dyn();

    free(q);
    stat_memoire_dyn();

    free(w);
    stat_memoire_dyn();

    free(k);
    stat_memoire_dyn();
    printf("Allocations et déallocations réussies!");
} /*test_alloc_1 */

/*
 * fonction de test de l'allocation et de la libération mémoire
 * avec affichage a chaque opération de la cartographie du tas
 * on teste le dépassement de la taille allouable
 */
static void test_malloc_2(){
    char *q, *w;
    /* test malloc et free */

    q=(char *)malloc((MAX_MEMORY/2)*sizeof(char));
    test_pointeur((void *) q, CONTINUER);
    stat_memoire_dyn();

    w=(char *)malloc((MAX_MEMORY/2)*sizeof(char));
    test_pointeur((void *) w, CONTINUER);
    stat_memoire_dyn();

    free(q);
    stat_memoire_dyn();

    printf("Allocations et déallocations réussies!");
} /*test_alloc_2 */


/*
 * fonction de test du calloc et remplissage de valeur
 * et de la libération mémoire
 * avec affichage à chaque opération de la cartographie du tas
 * à la fin des libérations on doit avoir le bon même nombre d'octets
 * libres qu'au début
 */
static void test_calloc_1(){
    int *tab;
    int nb = 100;
    int i;
    tab = (int *)calloc(nb,sizeof(int));
    for(i=0;i<nb;i++)
        tab[i]=i;
    stat_memoire_dyn();
    free((void *) tab);
    stat_memoire_dyn();
    printf("Callocations et déallocations réussies!");
}

/*
 * test de dépassement de la taille
 * on alloue plus que ce que nous avons
 * dans le tableau de caractères
 */
static void test_calloc_2(){
    char *tab1, *tab2;
    /* ce 1er appel doit passer */
    tab1 = (char *)calloc(MAX_MEMORY/2,sizeof(char));

    test_pointeur((void *) tab1, CONTINUER);
    stat_memoire_dyn();

    /* ici ça ne doit pas passer*/
    tab2 = (char *)calloc(MAX_MEMORY/2,sizeof(char));
    test_pointeur((void *) tab2, CONTINUER);

    free((void *) tab1);
    stat_memoire_dyn();

    printf("Callocations et déallocations réussies!");
}

/*
 * test realloc avec une taille plus petite visible dans la 1ère zone
 */
static void test_realloc_1(){
    int *tab1, *tab2;
    int k;
    /* test d'un pointeur qui pointe hors de la zone du tas => message d'erreur et retour NULL */
    tab1 = &k;
    tab1 = (int *) realloc((void *) tab1,500*sizeof(int));
    test_pointeur((void *) tab1, CONTINUER);

    /* test en réappelant avec la même taille => on retourne le ptr passé en paramètre  */
    tab1 = (int *)calloc(1000,sizeof(int));
    test_pointeur((void *) tab1, CONTINUER);
    tab1 = (int *) realloc((void *) tab1,1000*sizeof(int));


    /* 2ème appel doit passer*/
    tab2 = (int *)calloc(1000,sizeof(int));
    test_pointeur((void *) tab2, CONTINUER);
    stat_memoire_dyn();

    tab1 = (int *) realloc((void *) tab1,500*sizeof(int));
    stat_memoire_dyn();

#ifdef Y675
#endif
    printf("reallocations réussies!");
}

/*
 * test du realloc dans lequel on augmebte
 * la taille dont on a besoin et on pourra
 * aller la chercher dans le bloc suivant
 * qui est un bloc libre => zone étendue
 */
static void test_realloc_2(){
    char *tab1, *tab2, *tab3;
    int nb = 1000;

    /* 1er appel de 1000 entier */
    tab1 = (char *)calloc(nb,sizeof(char));
    test_pointeur((void *) tab1, CONTINUER);

    /* 2ème appel de 1000 entier */
    tab2 = (char *)calloc(nb,sizeof(char));
    test_pointeur((void *) tab2, CONTINUER);


    /* t2ème appel de 1000 entier  */
    tab3 = (char *)calloc(nb,sizeof(char));
    test_pointeur((void *) tab3, CONTINUER);
    stat_memoire_dyn();

    /* on libére l'espace du milieu tab2 */
    free((void *) tab2);
    stat_memoire_dyn();

    /*
     * je tente maintenant de réallouer tab1 + grand, il doit recupérer
     * un peu d'espace libre dans le bloc suivant qui esy libre
     */
    tab1 = (char *) realloc((char *) tab1,(nb+20)*sizeof(char));
    stat_memoire_dyn();

    printf("reallocations réussies!");
}

/*
 * test du réalloc avec déplacement du tableau
 * et vérification si déplacement effectué
 */
static void test_realloc_3(){
    int *tab1, *tab2;
    int nb = 1000;
    int i;

    /* test un réalloc avec déplacement */
    /* 1er appel de 1000 entier */
    tab1 = (int *)calloc(nb,sizeof(int));
    test_pointeur((void *) tab1, CONTINUER);
    /* initialisation du tableau */
    for(i=0;i<nb;i++)
        tab1[i]=i;

    /* 2ème appel de 1000 entier */
    tab2 = (int *)calloc(nb,sizeof(int));
    test_pointeur((void *) tab2, CONTINUER);

    /* on libére l'espace tab1 */
 /*   free((void *) tab1);
    stat_memoire_dyn(); */

    /* je tente maintenant de réallouer tab1 + grand => avec déplacemennt */
    tab1 = (int *) realloc((void *) tab1,(nb+20)*sizeof(int));
    stat_memoire_dyn();

    /* vérification des valeurs après déplacement */
    for(i=0;i<nb;i++)
        if (tab1[i] !=i) fprintf(stdout,"Realloc : pb recopie valeur tableau");

    printf("reallocations réussies!");
}

/*
 * on tente d'agglomérer une zone aui a exactelmebt la taille demandée
 */
static void test_realloc_4(){
    char *tab1, *tab2, *tab3;
    int nb = 1000;

    /* 1er appel de 1000 entier */
    tab1 = (char *)calloc(nb,sizeof(char));
    test_pointeur((void *) tab1, CONTINUER);

    /* 2ème appel de 1000 entier */
    tab2 = (char *)calloc(20,sizeof(char));
    test_pointeur((void *) tab2, CONTINUER);


    /* t2ème appel de 1000 entier  */
    tab3 = (char *)calloc(nb,sizeof(char));
    test_pointeur((void *) tab3, CONTINUER);
    stat_memoire_dyn();

    /* on libére l'espace du milieu tab2 */
    free((void *) tab2);
    stat_memoire_dyn();

    /*
     * je tente maintenant de réallouer tab1 + grand, il doit recupérer
     * un peu d'espace libre dans le bloc suivant qui esy libre
     */
    tab1 = (char *) realloc((char *) tab1,(nb+20)*sizeof(char));
    stat_memoire_dyn();

    printf("reallocations réussies!");
}


void procedure_test(Type_test t,int nb){
    switch(t){
        case MALLOC:
            switch(nb){
                case 1 : test_malloc_1();
                        break;
                case 2 :test_malloc_2();
                        break;
            }
            break;
        case CALLOC:
            switch(nb){
                case 1 : test_calloc_1();
                    break;
                case 2 :test_calloc_2();
                    break;
            }
            break;
        case REALLOC:
            switch(nb){
                case 1 : test_realloc_1();
                    break;
                case 2 :test_realloc_2();
                    break;
                case 3 :test_realloc_3();
                    break;
                case 4 :test_realloc_4();
                    break;
            }
            break;
    }
}
