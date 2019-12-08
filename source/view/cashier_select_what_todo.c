#include <stdlib.h>
#include <string.h>

#include "utils/logger.h"
#include "view/views.h"
#include "utils/input.h"
#include "utils/string.h"
#include "shop/clients.h"

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

void cashier_select_what_todo(Session *session, StockList *stocks, ClientsList *clients)
{
    if (session->client == NULL)
    {
        cashier_input_card_id(session, clients);
    }

    const char *choices[] = {
        "Effectuer un achat",
        "Rendre des bouteilles consignées",
        "Sortir du programme",
        NULL,
    };

    char greeting[200];
    if (session->client != NULL)
        sprintf(greeting, "Bonjour %s %s, veuillez faire un choix", session->client->firstname, session->client->lastname);
    else
        sprintf(greeting, "Bonjour veuillez faire un choix");

    bool exited = false;

    do
    {
        switch (user_select(NULL, greeting, choices))
        {
        case 0:
            log_info("Vous avez choisi d'effectuer un achat");
            cashier_scan_items(session, stocks);
            break;

        case 1:
            log_info("Vous avez choisi de rendre des bouteilles consignées");
            cashier_return_consigned_bottles(session, stocks);
            break;

        case 2:
            session_print_bill(session);
            exited = true;
            break;
        };
    } while (!exited);
}
