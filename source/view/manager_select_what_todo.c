
#include <stdlib.h>
#include <string.h>

#include "model/view.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "utils/string.h"
#include "view/views.h"

#include "shop/clients.h"
#include "shop/stocks.h"
#include "shop/users.h"

void manager_select_what_todo(User *user, UsersList *users, StockList *stock, ClientsList *clients)
{
    const char *choices[] = {
        "Liste des produits",
        "Liste des clients",
        "Liste des employers",
        "Retour au menu principal",
        NULL,
    };

    bool exited = false;

    do
    {
        switch (user_select("Selectionnez une interface", choices))
        {
        case 0:
            model_view(user, "Liste des produits", stocks_model_create(), stock);
            break;

        case 1:
            model_view(user, "Liste des clients", clients_model_create(), clients);
            break;

        case 2:
            model_view(user, "Liste des employers", users_model_create(), users);
            break;

        case 3:
            exited = true;
            break;

        default:
            break;
        }
    } while (!exited);
}