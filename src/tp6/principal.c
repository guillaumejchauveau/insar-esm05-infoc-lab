#include <stdio.h>
#include <memory.h>
#include "fichier.h"
#include "traitement.h"

int affichage(char *ligne) {
  printf("%s", ligne);
  return 0;
}

int affichage2(char *ligne) {
  static c = 1;
  printf("%d [%lu] %s", c++, strlen(ligne), ligne);
  return 0;
}

int lireProfession(char * str){
  char prof[TMAX];		/* Variable de récupération d’un nom */
  char reste[TMAX];	/* Variable de récupération des fins de ligne */

  if (sscanf(str," 1 OCCU %[^/\r\n]%[ \r\n]", prof, reste)==2){
    printf("%s\n",prof);
  }
  return 0;
}

int main() {
  FILE *pFile = NULL;  /* Descripteur du fichier */

  pFile = ouvrirFichier("../../src/tp6/exemple.ged", "r", ARRET);
  traiterLignesFichier(pFile, &lireProfession);
  fermerFichier(pFile);
  return 0;
}
