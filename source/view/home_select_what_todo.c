#include <string.h>

#include "utils/input.h"
#include "utils/logger.h"
#include "utils/terminal.h"
#include "view/views.h"

void home_select_what_todo(User *user, UsersList *users, StockList *stock, ClientsList *clients)
{
    const char *choices[] = {
        "Interface manager",
        "Interface client",
        " Sortir du programme",

        NULL,
    };

    do
    {
        switch (user_select(user, "Selectionnez une interface", choices))
        {
        case 0:
            manager_select_what_todo(user, users, stock, clients);
            break;

        case 1:
        {

            Basket *basket = basket_create(stock, cashier_input_card_id(clients));

            log_info("Bonjour et bienvenue chez Colruyt");

            cashier_select_what_todo(user, basket, stock, clients);
            float total = basket_bill(basket, stdout);
            if (basket->owner)
            {
                basket->owner->points += total / 10;
            }

            terminal_read_key();

            basket_destroy(basket);
            break;
        }

        case 2:
            log_info("Bye bye :)");

        default:
            return;
        }
    } while (1);
}