#include "view/views.h"
#include "utils/input.h"
#include "utils/logger.h"

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
        log_info("Vous avez choisi d'effectuer un achat");
        cashier_select_what_todo(stocks);
        break;

    case 1:
        log_info("Vous avez choisi de rendre des bouteilles consignées");
        cashier_return_consigned_bottles(stocks);
        break;

    case 2:
        log_info("Bye bye :)");

    default:
        break;
    }
}