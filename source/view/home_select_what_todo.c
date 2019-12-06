#include <string.h>

#include "view/views.h"
#include "utils/input.h"
#include "utils/logger.h"

void home_select_what_todo(User *user, StockList *stock, ClientsList *clients)
{
    const char *choices[] = {
        "Interface manager",
        "Interface client\n",
        "Sortir du programme",

        NULL,
    };

    do
    {
        switch (user_select("Selectionnez une interface", choices))
        {
        case 0:
            manager_select_what_todo(user, stock, clients);
            break;

        case 1:
        {
            Session *session = session_create();

            log_info("Bonjour et bienvenue chez Colruyt");
            cashier_select_what_todo(session, stock, clients);

            session_destroy(session);
            break;
        }

        case 2:
            log_info("Bye bye :)");

        default:
            return;
        }
    } while (1);
}