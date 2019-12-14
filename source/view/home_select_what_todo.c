#include <string.h>

#include "model/view.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "utils/terminal.h"
#include "view/views.h"

void home_select_what_todo(User *user, UsersList *users, StockList *stock, ClientsList *clients)
{
    const char *choices[] = {
        "Interface caissier",

        "Liste des produits",
        "Liste des clients",
        "Liste des employés",

        " Sortir du programme",

        NULL,
    };

    do
    {
        switch (user_select(user, "Selectionnez une interface", choices))
        {
        case 0:
        {
            Basket *basket = basket_create(stock, cashier_input_card_id(clients));

            log_info("Bonjour et bienvenue chez Colruyt");

            cashier_select_what_todo(user, basket, stock);

            basket_destroy(basket);
            break;
        }

        case 1:
            model_view(user, "Liste des produits", stocks_model_create(), stock);
            break;

        case 2:
            model_view(user, "Liste des clients", clients_model_create(), clients);
            break;

        case 3:
            model_view(user, "Liste des employés", users_model_create(), users);
            break;

        default:
            log_info("Bye bye :)");
            return;
        }
    } while (1);
}