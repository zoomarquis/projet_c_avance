#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "fin_partie.h"

// faire généricité (passer parametre -1 -1, 1 1 ... pour connaitre calculer déplacement)

bool testAlignement(Plateau plateau, unsigned l, unsigned c, int deplaL, int deplaC)
{
    assert(l >= 0 && l < NB_LIGNE);
    assert(c >= 0 && c < NB_COLONNE);
    assert(deplaL == 1 || deplaL == -1 || deplaL == 0);
    assert(deplaC == 1 || deplaC == -1 || deplaC == 0);
    unsigned nb_aligne = 1;
    Case valeur = plateau[l][c];
    assert(valeur == J1 || valeur == J2);
    unsigned posL = l
}

bool testAlignementHorizontal(Plateau plateau, unsigned l, unsigned c)
{
    unsigned nb_aligne = 1;
    Case valeur = plateau[l][c];
    unsigned pos = c - 1;
    while (pos >= 0 && plateau[l][pos--] == valeur) // vers la gauche
        nb_aligne++;
    pos = c + 1;
    while (pos < NB_COLONNE && plateau[l][pos++] == valeur) // vers la gauche
        nb_aligne++;
    return nb_aligne >= 4;
}

bool testAlignementVertical(Plateau plateau, unsigned l, unsigned c)
{
    unsigned nb_aligne = 1;
    Case valeur = plateau[l][c];
    unsigned pos = l - 1;
    while (pos >= 0 && plateau[pos--][c] == valeur) // vers le haut
        nb_aligne++;
    pos = l + 1;
    while (pos < NB_LIGNE && plateau[pos++][c] == valeur) // vers le bas
        nb_aligne++;
    return nb_aligne >= 4;
}

bool testDiagonaleHautGaucheBasDroite(Plateau plateau, unsigned l, unsigned c)
{
    unsigned nb_aligne = 1;
    Case valeur = plateau[l][c];
    unsigned posl = l - 1, posc = c - 1;
    while (posl >= 0 && posc >= 0 && plateau[posl--][posc--] == valeur) // vers le haut gauche
        nb_aligne++;
    posl = l + 1;
    posc = c + 1;
    while (posl < NB_LIGNE && posc < NB_COLONNE && plateau[posl++][posc++] == valeur) // vers le bas droite
        nb_aligne++;
    return nb_aligne >= 4;
}

bool testDiagonaleHautDroiteBasGauche(Plateau plateau, unsigned l, unsigned c)
{
    unsigned nb_aligne = 1;
    Case valeur = plateau[l][c];
    unsigned posl = l - 1, posc = c + 1;
    while (posl >= 0 && posc < NB_COLONNE && plateau[posl--][posc++] == valeur) // vers le haut droite
        nb_aligne++;
    posl = l + 1;
    posc = c - 1;
    while (posl < NB_LIGNE && posc >= 0 && plateau[posl++][posc--] == valeur) // vers le bas gauche
        nb_aligne++;
    return nb_aligne >= 4;
}

bool testFinPartie(Plateau plateau, unsigned l, unsigned c, unsigned nb_jetons)
{
    return nb_jetons == (NB_LIGNE * NB_COLONNE) || testAlignementHorizontal(plateau, l, c) || testAlignementVertical(plateau, l, c) ||
           testDiagonaleHautDroiteBasGauche(plateau, l, c) || testDiagonaleHautGaucheBasDroite(plateau, l, c);
}