#include "view/views.h"

void select_what_todo(StockList *stocks)
{
    int choice = -1;

    printf("1 : Effectuer un achat\n2 : Rendre des bouteilles consignées \n3 : Sortir du programme\n\n");

    while (choice < 0 || choice > 3)
    {
        printf("> ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // Effectuer un achat
            printf("vous avez choisi d'effectuer un achat\n");
            break;

        case 2: // Rendrer des bouteilles consignées
            printf("vous avez choisi de rendre des bouteilles consignées\n");
            return_consigned_bottles(stocks);
            break;

        case 3: // Fin du programme
            break;

        default:
            printf("Erreur, recommencez\n");
        }
    }
}