#include <stdlib.h>
#include <string.h>
#include <time.h>

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
            if (itemInStocks->reduction != 0)
            {
                printf("%3d %% \n", itemInStocks->reduction);
            }
            else
                printf("\n");
        }
    }
}

void authentication(ClientsList *clients)
{
    BareCode inserted_id;
    char input_client_id[5], input_client_str[60];
    Client *client = (Client *)malloc(sizeof(Client));

    const char *choices[] = {
        "S'authentifier",
        "Créer un compte client Colruyt",
        "Continuer sans compte",
        NULL,
    };

    switch (user_select("Cher client, vous pouvez profiter des points extra grace au compte client Colruyt", choices))
    {
    case 0:
        do
        {
            user_input("Inserez votre ID client ", "####", input_client_id);
            inserted_id = atoi(input_client_id);
            client = client_lookup(clients, inserted_id);
            if (client == NULL)
            {
                printf("Erreur, Ce compte n'existe pas");
                if (!(user_yes_no("Erreur, identifiant incorrect, voulez-vous reessayer ? ")))
                {
                    authentication(clients);
                    break;
                }
            }
        } while (client == NULL);
        printf("Bonjour, %s %s !\n", client->firstname, client->lastname);

        break;

    case 1:
        do
        {
            user_input("Inserez votre nom ", "......................", input_client_str);
            strcpy(client->lastname, input_client_str);
            user_input("Inserez votre prenom ", "......................", input_client_str);
            strcpy(client->firstname, input_client_str);
            user_input("Inserez votre email ", "......................", input_client_str);
            strcpy(client->email, input_client_str);

            printf("Vos informations :\n%s %s\n%s\n\n", client->firstname, client->lastname, client->email);

        } while (!(user_yes_no("Confirmer ?")));

        srand(time(NULL));
        client->id = rand() % 9999;
        client->points = 0;

        // TODO : add the client to the dat file
        break;

    case 2:
        log_info("");
    }
}

void cashier_select_what_todo(Session *session, StockList *stocks, ClientsList *clients)
{
    (void)clients;

    const char *choices[] = {
        "Effectuer un achat",
        "Rendre des bouteilles consignées",
        "Sortir du programme",
        NULL,
    };

    switch (user_select("Veuillez faire un choix", choices))
    {
    case 0:
        log_info("Vous avez choisi d'effectuer un achat");
        authentication(clients);
        cashier_scan_items(session, stocks);
        break;

    case 1:
        log_info("Vous avez choisi de rendre des bouteilles consignées");
        cashier_return_consigned_bottles(stocks);
        break;

    case 2:
        log_info("Bye bye :)");

    default:
        break;
    };
}
