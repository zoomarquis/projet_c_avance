/**
 * @file ia.h
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Définition de la fonction de création des IA.
 * @version 1.0
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

Joueur *makeIA(Type, char);

#endif