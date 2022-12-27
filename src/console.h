/**
 * @file console.h
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Définition des fonctions de création du mode d'interface console.
 * @version 1.0
 * @date 2022-12-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef CONSOLE_H
/**
 * @def CONSOLE_H
 * @brief la garde
 */
#define CONSOLE_H

#include "puissance_quatre.h"

userInterface *makeConsole();
Joueur *makeHumainConsole(Type);

#endif