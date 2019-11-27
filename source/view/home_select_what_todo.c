#include "view/views.h"
#include "utils/input.h"

void home_select_what_todo(StockList *stocks)
{
    const char *choices[] = {
        "Effectuer un achat",
        "Rendre des bouteilles consignées",
        "Sortir du programme",
        NULL,
    };

    switch (user_select("Donc on fait quoi", choices))
    {
    case 0:
        printf("vous avez choisi d'effectuer un achat\n");
        cashier_select_what_todo(stocks);
        break;

    case 1:
        printf("vous avez choisi de rendre des bouteilles consignées\n");
        cashier_return_consigned_bottles(stocks);
        break;

    case 2:
        printf("Bye bye :)\n");

    default:
        break;
    }
}