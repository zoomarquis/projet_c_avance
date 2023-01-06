/**
 * @file ia.h
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Définition de la fonction de création des IA.
 * @version 0.1
 * @date 2022-12-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef IA_H
/**
 * @def IA_H
 * @brief la garde
 */
#define IA_H

#include "puissance_quatre.h"

unsigned valeurCase(Puissance4, unsigned, unsigned);
unsigned autour(Puissance4, unsigned, unsigned);
unsigned scoreJoueur(Puissance4);
int evaluation(Puissance4 *);
Joueur *makeIA(Type, char);

#endif