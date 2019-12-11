#include <stdlib.h>
#include <string.h>

#include "model/view.h"
#include "shop/clients.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "utils/string.h"
#include "view/views.h"

#include "shop/basket.h"

void autocomplete_stock_list(const char *user_input, StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;

        char item_id_string[5];
        sprintf(item_id_string, "%03d", itemInStocks->id);

        if (str_start_with(user_input, item_id_string))
        {
            printf("%04d %s %5.2f€ ", itemInStocks->id, itemInStocks->label, itemInStocks->price);
            if (itemInStocks->discount != 0)
            {
                printf("%3d %%", itemInStocks->discount);
            }
            printf("\n");
        }
    }
}

void cashier_select_what_todo(User *user, Basket *basket, StockList *stocks, ClientsList *clients)
{
    if (basket->owner == NULL)
    {
        basket->owner = cashier_input_card_id(clients);
    }

    const char *choices[] = {
        "Effectuer un achat",
        "Rendre des bouteilles consignées",
        "Afficher le panier",
        " Retour",
        NULL,
    };

    char greeting[200];
    if (basket->owner != NULL)
    {
        sprintf(greeting, "Bonjour %s %s, veuillez faire un choix", basket->owner->firstname, basket->owner->lastname);
    }
    else
    {
        sprintf(greeting, "Bonjour veuillez faire un choix");
    }

    bool exited = false;

    do
    {
        switch (user_select(user, greeting, choices))
        {
        case 0:
            log_info("Vous avez choisi d'effectuer un achat");
            cashier_scan_items(basket, stocks);
            break;

        case 1:
            log_info("Vous avez choisi de rendre des bouteilles consignées");
            cashier_return_consigned_bottles(basket, stocks);
            break;

        case 2:
        {
            model_view(user, "Panier", basket_model_create(), basket);
            break;
        }
        default:
            exited = true;
            break;
        };
    } while (!exited);
}
