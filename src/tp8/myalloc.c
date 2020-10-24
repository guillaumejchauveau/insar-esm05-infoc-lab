#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>
#include <stdarg.h>

#include "myalloc.h"

/*
 * mon espace mémoire réservé pour l'allocation dynamique
 * déclarée en static pour éviter toyt accès hors
 * des fonctions du module
 */
/*
 * la tas est est tableau de char qui contient
 * à la fois les données allouées et les descripteurs de bloc
 */
static char mon_tas[MAX_MEMORY];
static Descript_mem *liste_bloc_mem = (Descript_mem *) mon_tas;

/* prototype des fonctions 'static' de service */

/* indique si une zone mémoire est du statut (LIBRE ou OCCUPE) passé en paramètre */
static int statut_zone(Descript_mem *zone, Statut_memoire statut);

/* donne l'indice du tableau de caractère correspndant au pointeur ptr */
static int indice_ptr(char *ptr);

/* initialise la mémoire dynamique (effectué au 1er appel de Mon_malloc() */
static void init_memoire_dyn();

/* decoupe un bloc en  2, le premier faisant nb_octet le suivant le reste moins la taille du descripteur */
static void decoupe_bloc(Descript_mem *bloc_courant,size_t nb_octets);

/* vient augmenter de nb_octets la taille du bloc courant qui est suivit d'un bloc libre */
static void etend_bloc(Descript_mem *bloc_courant,size_t nb_octets);

/* scanne la mémoire afin de merger les blocs libres */
static void regroupement_libre();

/* vérifie sin un pointeur pointe bien dans la zone dynamique */
static int est_dans_tas(void* ptr);

/* pour un pointeur résultat d'une allocation par malloc, calloc ou reaclloc, retourne son descripteur */
static Descript_mem *ptr_descripteur(void *ptr_data);


/*
 * petite fonction variadique pour n'afficher les messages de debug
 * que si DEGUG est défini (dans myalloc.h)
 */
void debug_print(char *format,...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
#endif
} /* debug_print */

void stat_memoire_dyn() {
    Descript_mem *curr;
    int cpt = 0; /* compteur de zone */
    long taille_totale = 0;
    curr=liste_bloc_mem;
#ifdef ALLOC_PERSO
    /* la cartographie n'a de sens que si on utilise nos fonctions d'allocation */
    debug_print("debut statistique !\n");
    fprintf(stdout,"----------------------------------------------------\n");
    fprintf(stdout,"!          début cartographie mémoire              !\n");
    fprintf(stdout,"----------------------------------------------------\n");
    while(curr!=NULL){
        fprintf(stdout,"zone n°%d\n",cpt++);
        fprintf(stdout,"\tla zone est %s\n",((statut_zone(curr,LIBRE))?"LIBRE":"OCCUPE"));
        fprintf(stdout,"\ttaille de la zone utile : %ld octets\n",curr->size);
        fprintf(stdout,"\ttaille de la zone avec service : %ld octets\n",curr->size+sizeof(Descript_mem));
        fprintf(stdout,"\tindice zone mémoire [%d, %d]\n", indice_ptr((char *) (curr+1)),
                indice_ptr(((char *) (curr+1))+curr->size-1));
        /* on cumule la taille du descripteur + la taille utile */
        taille_totale += sizeof(Descript_mem) + curr->size;
        curr=curr->suiv;
    }
    fprintf(stdout,"taille totale de la mémoire dynamique (données + descripteurs) : %ld octets\n",taille_totale);
    debug_print("fin statistique !\n");
#endif
} /* stat_memoire_dyn */


/*
 * fonction qui determine si un bloc mémoire est libre ou occupé
 */
static int statut_zone(Descript_mem *zone, Statut_memoire statut) {
    return (zone->statut == statut);
} /* statut_zone */

/*
 * fonction qui à partir d'un pointeur qui pointe sur le tas
 * donne l'indice dans le tableau de caractère
 * résultat -1 si pointeur NULL ou hors tableau
 */
static int indice_ptr(char *ptr) {
    int result = -1;
    if (ptr != (char *) NULL) {
        result = (int) (ptr-mon_tas);
        if ((result <0) || (result >= MAX_MEMORY))
            result = -1;
    }
    return (result);
} /* indice_ptr */

/*
 * fonction d'affichage de la cartographie mémoire
 */


/*
 * initialisaition de la structure du 1er grand bloc libre
 */
static void init_memoire_dyn(){
    liste_bloc_mem->size=MAX_MEMORY-sizeof(Descript_mem);
    liste_bloc_mem->statut=LIBRE;
    liste_bloc_mem->suiv= (Descript_mem *)NULL;
    debug_print("initialisation mémoire dynamique effectuée !\n");
} /* init_memoire_dyn */

/*
 * découpe le bloc libre passé en paramètre en 2
 *  - le premier sera un bloc de nb_octets utiles avec statut occupé
 *  - le 2ème sera un bloc de la taille du reste avec statut libre
 *  Aucune vérification effectuée, c'est de la responsabilité de l'appelant
 */
static void decoupe_bloc(Descript_mem *bloc_courant,size_t nb_octets){
    /* calcul de la taille totale : nb_octet + taille du bloc de service chainage */
    size_t nb_octet_total = nb_octets+sizeof(Descript_mem);

    /*
     * création du bloc mémoire supplémentaire qui sera placé derrière le bloc_courant
     * le cast en (char *) sert a effectuer un calcul (arithmetique sur pointeurs) en octets
     * pour "mapper" l'adresse du descripteur du nouveau bloc dans le tableau
     * le cast en (Block_mem *) sert à accorder les types
     */
    Descript_mem *nouveau_bloc=(Descript_mem *)((char *)bloc_courant+nb_octet_total);
    debug_print("découpage d'un bloc en 2 : un bloc de %d et un bloc de %d!\n",
                nb_octets,(bloc_courant->size)-nb_octets-sizeof(Descript_mem));
    /* initialisation des paramètres de ce nouveau bloc (qui sera libre) dont chaînage */
    nouveau_bloc->size=(bloc_courant->size)-nb_octets-sizeof(Descript_mem);
  /*  nouveau_bloc->size=(bloc_courant->size)-nb_octets; */
    nouveau_bloc->statut=LIBRE;
    nouveau_bloc->suiv=bloc_courant->suiv;

    /* mise à jour du bloc courant (taille et chaînage) ce bloc sera occupé */
    bloc_courant->size=nb_octets;
    bloc_courant->statut=OCCUPE;
    bloc_courant->suiv=nouveau_bloc;
} /* decoupe_bloc */

/*
 * étend un bloc occupé de nb_octets sachant qu'il est
 * suivi d'un bloc libre de taille suffisante
 *  - le premier occupé augmentation de nb_octets
 *  - le bloc suivant libre diminué de nb_octets
 * Aucune vérification effectuée, c'est de la responsabilité
 * de l'appelant de faire les vérifs
 */
static void etend_bloc(Descript_mem *bloc_courant,size_t nb_octets){
    /* sauvegarde des valeurs intermédiaires du bloc libre suivant */
    size_t taille_suiv = bloc_courant->suiv->size;
    Descript_mem *bloc_suiv = bloc_courant->suiv->suiv;
    /*
     * test pour savoir si la taille demandée est très exactement la même
     * que la taille de la zone libre : c'est un cas particulier
     */
    if (nb_octets == bloc_courant->suiv->size) {
        /*
         * traitement particulier :
         *  - le bloc a exactement la bonne taille en taille utile
         *  - on est tenté de ne faire qu'une zone en tout
         *  - elle sera trop grande de sizeof(Descript_mem)
         *  mais ce n'est pas grave car on le recupérera au free
         */
        /* mise à jour des champs du bloc courant pour aggréger ce bloc vide */
        bloc_courant->size += nb_octets+sizeof(Descript_mem); /* ! on ajoute la taille du descipteur */
        bloc_courant->statut = OCCUPE;
        bloc_courant->suiv = bloc_suiv;
    } else {

        /*
         * création du bloc mémoire supplémentaire qui sera placé derrière le bloc_courant
         * le cast en (char *) sert a effectuer un calcul (arithmetique sur pointeurs) en octets
         * pour "mapper" l'adresse du descripteur du nouveau bloc dans le tableau
         * le cast en (Block_mem *) sert à accorder les types
         */

        Descript_mem *nouveau_bloc = (Descript_mem *) ((char *) bloc_courant->suiv + nb_octets);
        debug_print("extension d'un bloc occupé de %d à bloc de %d!\n",
                    bloc_courant->size, bloc_courant->size + nb_octets);
        /* initialisation des paramètres de ce nouveau bloc
         * avec les valeurs sauvegardées mises à jour
         */
        nouveau_bloc->size = taille_suiv - nb_octets;
        /*  nouveau_bloc->size=(bloc_courant->size)-nb_octets; */
        nouveau_bloc->statut = LIBRE;
        nouveau_bloc->suiv = bloc_suiv;

        /*
         * mise à jour du bloc courant (taille et chaînage)
         * ce bloc sera occupé (en fait il l'état déjà)
         */
        bloc_courant->size += nb_octets;
        bloc_courant->statut = OCCUPE;
        bloc_courant->suiv = nouveau_bloc;
    }
} /* etend_bloc */

/*
 * deux zones libres successives sont mergées
 */
static void regroupement_libre(){
    Descript_mem *curr,*prev;
    curr=liste_bloc_mem;
    debug_print("debut regroupement !\n");

    while((curr->suiv)!=NULL){
        if((statut_zone(curr, LIBRE)) && (statut_zone(curr->suiv, LIBRE))){
            debug_print("regroupement zones1 (%ld octets) + zone2 (%ld octets) + zone service (%ld octets) = %ld octets\n",
                    curr->size, curr->suiv->size,sizeof(Descript_mem), curr->size + curr->suiv->size + sizeof(Descript_mem));
            curr->size+=(curr->suiv->size)+sizeof(Descript_mem);
            curr->suiv=curr->suiv->suiv;
        } else {
            /*
             * on bloque la progression dans la liste pour vérifier si avec la même zone mergée,
             * la suivante ne peux pas aussi être regroupée d'où ces instructions que dans le else
             * en fait on ne fait pas du sur place car on a quand même virtuellement progressé
             * en regroupant les 2 zones
             */
            if (curr->suiv != NULL)  {
                prev = curr;
                curr = curr->suiv;
            }
        }

    }
    debug_print("fin regroupement !\n");
} /* regroupement_libre */


/*
  * vérifie si le pointeur passe en paramètre
  * est bien dans la zone de memoire dynamique
  * (i.e. s'il pointe bien vers un élément du tableau de caractère)
  */
static int est_dans_tas(void* ptr){
    return (((void*)mon_tas<=ptr)&&(ptr<(void*)(mon_tas+MAX_MEMORY)));
} /* est_dans_tas */


/*
 * détermination du pointeur sur le descripteur de zone mémoire
 * à partir du pointeur sur la zone de donnée
 */
static Descript_mem *ptr_descripteur(void *ptr_data) {
    /*
       * on va faire en sorte que curr pointe sur le descripteur de la zone mémoire
       * pointee par ptr, cette zone se trouve juste avant la zone pointée dans le tas
       * c'est donc le pointeur : ptr_data - la taille d'un descripteur de zone mémoire
       */
    Descript_mem* resultat= (Descript_mem*) ptr_data;
    --resultat;              /*  voila, c'est fait */
    return(resultat);
}


/*
 * fonction de test d'un pointeur
 */
void test_pointeur(void * t, Diag_ptr d) {
    if (t == (void *) NULL){
        if (d == ARRET){
            fprintf(stdout,"retour d'un pointeur NULL - arrêt du programme\n");
            exit(45);
        } else fprintf(stdout,"retour d'un pointeur NULL\n");
    }
} /* test_pointeur */


/*
 * fonction Mon_alloc
 * remplace le malloc de la stdlib
 * avec les mêmes paramètres que malloc
 * et le même fonctionnement
 */
void *Mon_malloc(size_t nb_octets){
   void *resultat;
   Descript_mem *curr,*prev;
   static int cpt = 0;

   if(!cpt){
       /* 1ère allocation dynamique => initialisation du tas */
       init_memoire_dyn();
       cpt++;
   }
   if (nb_octets == 0)
       /* on suit ce que dit la specification */
       resultat = (void *) NULL;
   else {
       /* intialisation du parcours avec la liste chaînée des descripteurs de blocs mémoire */
       curr = liste_bloc_mem;

       /* boucle de recherche d'une zone libre dont la taille est suffisante */
       while ((((curr->size) < nb_octets) || statut_zone(curr, OCCUPE)) && (curr->suiv != NULL)) {
           prev = curr;
           curr = curr->suiv;
       }
       debug_print("Mon_alloc : on veut allouer %d octets, on a trouvé un bloc libre de %ld octets\n",
                   (int) nb_octets, curr->size);

       /* soit on a trouvé un espace soit il n'y a plus de place */
       if ((curr->size) == nb_octets) {
           /* il existe une zone qui a exactement la bonne taille ! on la prend */
           curr->statut = OCCUPE;
           resultat = (void *) (++curr);
           debug_print("Mon_alloc : on alloue exactement la bonne taille\n");
       } else if ((curr->size) > (nb_octets + sizeof(Descript_mem))) {
           /* on a trouvé un bloc plus grand => on le découpe en une zone occupée et une zone libre */
           decoupe_bloc(curr, nb_octets);
           resultat = (void *) (++curr);
           debug_print("Mon_alloc : on alloue un bloc après avoir coupé le bloc en 2\n");
       } else {
           /* hélas il n'y a plus assez d'espace mémoire */
           resultat = NULL;
           debug_print("Mon_alloc() : désolé, plus assez d'espace mémoire\n");
       }
   }
   return (resultat);
} /* Mon_alloc */


/*
 * fonction Mon_free
 * remplace le free de la stdlib
 * avec les mêmes paramètres que free
 */
void Mon_free(void* ptr){

  /* vos déclarations ici */

  debug_print("Mon_free() : début free\n");
  
  /* votre code ici */

  debug_print("Mon_calloc() : début free\n");
} /* Mon_free */


/*
 * fonction Mon_calloc
 * remplace le calloc de la stdlib
 * avec les mêmes paramètres que calloc
 */
void *Mon_calloc(size_t count, size_t size) {
   void *resultat = (void *) NULL;

  /* vos déclarations ici */

  debug_print("Mon_calloc() : début calloc\n");
  
  /* votre code ici */

  debug_print("Mon_calloc() : début calloc\n");
  
  return(resultat);
} /* Mon_calloc */


/*
 * fonction Mon_realloc
 * remplace le realloc de la stdlib
 * avec les mêmes paramètres que calloc
 */
void *Mon_realloc(void *ptr, size_t newsize) {
  void *resultat = (void *) NULL;
  /* vos déclarations ici */

  debug_print("Mon_realloc() : début realloc\n");
  
  /* votre code ici */
  
  debug_print("Mon_realloc() : début realloc\n");
  
  return(resultat);
} /* Mon_realloc */


