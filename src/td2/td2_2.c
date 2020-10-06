#include <stdio.h>
#include <stdlib.h>

#define N 10

//définition du type Action

//définition du type Ordre


typedef enum
{
  NORTH,
  EAST,
  SOUTH,
  WEST
} Direction;

typedef struct
{
  size_t x;
  size_t y;
  Direction direction;
} Mobile;

//prototypes des fonctions
void afficherMobile(Mobile *p_m);
int avancer(Mobile *p_m);
int tournerGauche(Mobile *p_m);
int tournerDroite(Mobile *p_m);


typedef enum
{
  AVANCER,
  TOURNERG,
  TOURNERD
} Action;

typedef struct
{
  size_t time;
  Action action;
} Ordre;

int donnerOrdre(Ordre *p_ordre, Mobile *p_m);
void afficherMobile(Mobile *p_m)
{
  printf("le mobile est en (%d,%d)\n", p_m->x, p_m->y);
}

int avancer(Mobile *p_m)
{
  switch (p_m->direction)
  {
  case NORTH:
  {
    if (p_m->x > 0)
      p_m->x--;
    else
      return 0;
    break;
  }
  case SOUTH:
  {
    if (p_m->x < (N - 1))
      p_m->x++;
    else
      return 0;
    break;
  }
  case EAST:
  {
    if (p_m->y < (N - 1))
      p_m->y++;
    else
      return 0;
    break;
  }
  case WEST:
  {
    if (p_m->y > 0)
      p_m->y--;
    else
      return 0;
    break;
  }
  default:
    printf("Direction inconnue dans avancer\n");
    return 0;
  }
  return 1;
}

/* Implementation des fonctiosn tournerGauche et tournerDRoite : à faire */

/* Programme principal */
int main()
{
  Ordre ordre0 = {0, AVANCER};
  Ordre ordre1 = {1, TOURNERD};
  Ordre ordre2 = {2, AVANCER};
  Mobile mobileM = {0, 1, EAST};
  afficherMobile(&mobileM);
  donnerOrdre(&ordre0, &mobileM);
  afficherMobile(&mobileM);
  donnerOrdre(&ordre1, &mobileM);
  afficherMobile(&mobileM);
  donnerOrdre(&ordre2, &mobileM);
  afficherMobile(&mobileM);
  return 0;
}

int tournerGauche(Mobile *p_m)
{
  p_m->direction--;
  if (p_m->direction < 0) {
    p_m->direction = WEST;
  }
  return 1;
}

int tournerDroite(Mobile *p_m)
{
  p_m->direction++;
  p_m->direction %= 4;
}

int donnerOrdre(Ordre *p_ordre, Mobile *p_m)
{
  switch (p_ordre->action) {
    case AVANCER:
      return avancer(p_m);
    case TOURNERD:
      return tournerDroite(p_m);
    case TOURNERG:
      return tournerGauche(p_m);
    default:
      return 0;
  }
}
