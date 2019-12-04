#include "view/views.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "model/view.h"

#include <string.h>
void home_select_what_todo(StockList *stock, ClientsList *clients)
{

    char input_manager[20];

    const char *choices[] = {
        "Interface manager",
        "Interface client",
        "Sortir du programme",
        NULL,
    };

    do
    {
        /* code */

        switch (user_select("Selectionnez une interface", choices))
        {
        case 0:
            do
            {
                user_input("Inserez votre identifiant manager", ".......", input_manager);
                if (strcmp(input_manager, "Nicolas") != 0)
                {
                    if (user_yes_no("Erreur, identifiant incorrect, voulez-vous reessayer ? ") == YES)
                    {
                        home_select_what_todo(stock, clients);
                        break;
                    }
                }
            } while (strcmp(input_manager, "Nicolas") != 0);

            manager_select_what_todo(stock, clients);
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