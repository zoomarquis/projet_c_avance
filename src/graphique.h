/**
 * @file graphique.h
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Définition des fonctions de création du mode d'interface graphique.
 * @version 1.0
 * @date 2022-12-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef GRAPHIQUE_H
/**
 * @def GRAPHIQUE_H
 * @brief la garde
 */
#define GRAPHIQUE_H

#include "puissance_quatre.h"

userInterface * makeGraphique();
Joueur * makeHumainGraphique(Type);

#endif