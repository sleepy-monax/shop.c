#include "view/views.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "model/model_view.h"

#include <string.h>
void home_select_what_todo(StockList *stocks, ClientsList *clients)
{

    char input_manager[20];

    const char *choices[] = {
        "Interface manager",
        "Interface client",
        "Liste des produits",
        "Liste des clients\n",
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
                        home_select_what_todo(stocks, clients);
                        break;
                    }
                }
            } while (strcmp(input_manager, "Nicolas") != 0);
            break;

        case 1:
        {
            Session *session = session_create();

            log_info("Bonjour et bienvenue chez Colruyt");
            cashier_select_what_todo(session, stocks, clients);

            session_destroy(session);
            break;
        }
        case 2:
            model_view("Liste des produits", stocks_model_create(), stocks);
            break;

        case 3:
            model_view("Liste des clients", clients_model_create(), clients);
            break;

        case 4:
            log_info("Bye bye :)");

        default:
            return;
        }
    } while (1);
}