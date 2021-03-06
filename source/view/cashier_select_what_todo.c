#include <stdlib.h>
#include <string.h>

#include "model/view.h"
#include "shop/basket.h"
#include "shop/clients.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "utils/string.h"
#include "utils/terminal.h"
#include "view/views.h"

void cashier_select_what_todo(User *user, Basket *basket, StockList *stocks)
{
    const char *choices[] = {
        "Effectuer un achat",
        "Rendre des bouteilles consignées",
        "Afficher le panier",
        "$ Payer",
        " Annuler",
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
        case 3:
        {
            if (basket->items->count == 0)
            {
                if (user_yes_no("Votre panier est vide, voulez-vous quitter ?", NO) == YES)
                {
                    break;
                }
            }
            else
            {

                if (basket->owner)
                {
                    if (user_yes_no("Voulez-vous payer avec vos points fidelitée?", YES))
                    {
                        basket->pay_with_point = true;
                    }
                }

                basket_pay(user, basket, stdout);

                terminal_read_key();
            }

            return;
        }
        default:
            return;
        };
    } while (true);
}
