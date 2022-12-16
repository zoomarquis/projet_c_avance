#ifndef PLATEAU_H
#define PLATEAU_H

#define NB_LIGNE 6
#define NB_COLONNE 7

typedef enum
{
    VIDE,
    J1,
    J2
} Case;

typedef Case Plateau[NB_LIGNE][NB_COLONNE];

#endif