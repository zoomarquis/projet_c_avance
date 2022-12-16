#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void printLigneIntermediaire()
{
    printf("\n");
    for (int c = 0; c < NB_COLONNE; c++)
        printf("+---");
    printf("+");
    printf("\n");
}

void printCase(Case v)
{
    switch (v)
    {
    case VIDE:
        printf("   |");
        break;
    case J1:
        printf(" X |");
        break;
    case J2:
        printf(" O |");
        break;
    }
}

void printPlateau(Plateau plateau)
{
    unsigned c, l;
    for (c = 0; c < NB_COLONNE; c++)
        printf("  %u ", c + 1);
    printLigneIntermediaire();

    for (l = 0; l < NB_LIGNE; l++)
    {
        printf("|");
        for (c = 0; c < NB_COLONNE; c++)
            printCase(plateau[l][c]);
        printf("  %u ", l + 1);
        printLigneIntermediaire();
    }
}

/** retourne -1 si la colonne n'est pas disponible
 * sinon retourne l'indice de la première case vide
 */
int testColonneDisponible(Plateau plateau, unsigned c)
{
    assert(c >= 0 && c < NB_COLONNE);
    for (int i = NB_LIGNE - 1; i >= 0; i--)
    {
        if (!(plateau[i][c]))
            return i;
    }
    printf("Cette colonne est pleine !\n");
    return -1;
}

/** retourne la colonne où il faut ajouter */
unsigned interaction(Plateau p, Case joueur)
{
    int coup;
    assert(joueur == J1 || joueur == J2);
    do
    {
        (joueur == J1) ? (printf("Joueur 1 : ")) : (printf("Joueur 2 : "));
        scanf("%hhd", &coup);
    } while (coup < 0 || coup >= NB_COLONNE || testColonneDisponible(plateau, coup) == -1);
    return coup;
}

void ajoutJeton(Plateau p, Case joueur, unsigned colonne, unsigned *nb_jeton)
{
    assert(joueur == J1 || joueur == J2);
    assert(testColonneDisponible(colonne) != -1);
    plateau[testColonneDisponible(colonne)][colonne] = joueur;
    *nb_jeton++;
}