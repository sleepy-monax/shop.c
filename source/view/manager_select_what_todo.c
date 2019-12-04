
#include <stdlib.h>
#include <string.h>

#include "utils/logger.h"
#include "view/views.h"
#include "utils/input.h"
#include "utils/string.h"
#include "shop/clients.h"
#include "model/model_view.h"

void manager_select_what_todo(StockList *stock, ClientsList *clients)
{
    const char *choices[] = {
        "Liste des produits",
        "Liste des clients",
        "Retour au menu principal",
        NULL,
    };

    bool exited = false;

    do
    {
        switch (user_select("Selectionnez une interface", choices))
        {
        case 0:
            model_view("🧺 Liste des produits", stocks_model_create(), stock);
            break;

        case 1:
            model_view("👥 Liste des clients", clients_model_create(), clients);
            break;

        case 2:
            exited = true;
            break;

        default:
            break;
        }
    } while (!exited);
}