#include <stdlib.h>

#include "view/views.h"
#include "utils/input.h"
#include "utils/string.h"

void autocomplete_stock_list_consigned(const char *user_input, StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;

        char item_id_string[5];
        sprintf(item_id_string, "%03d", itemInStocks->id);

        if (str_start_with(user_input, item_id_string) && itemInStocks->isConsigned)
        {
            printf("%04d %s %5.2f€\n", itemInStocks->id, itemInStocks->label, itemInStocks->consignedValue);
        }
    }
}

void return_consigned_bottles(StockList *stock)
{
    float totValue = 0.;
    int nb_bottles = 0;
    BareCode inserted_br;
    char choice = 'y', input[128];
    Item *found_item;

    while (choice == 'y')
    {
        printf("Inserez le codebarre de la bouteille a rendre : \n\n");
        stocks_display_consigned(stock);

        user_input("####", input, (ListCallback)autocomplete_stock_list_consigned, (void *)stock);
        inserted_br = atoi(input);

        found_item = stocks_lookup_item(stock, inserted_br);

        if (found_item != NULL && found_item->isConsigned)
        {
            printf("Entrez le nombre de bouteilles a rendre :\n> ");

            while (nb_bottles < 1)
            {
                scanf("%d", &nb_bottles);

                if (nb_bottles < 1)
                {
                    printf("Erreur, recommencez\n> ");
                }
            }

            totValue += found_item->consignedValue * nb_bottles;

            nb_bottles = 0;

            printf("Vous allez recuperer %5.2f€, voulez-vous continuer ? (y/n) :\n> ", totValue);

            choice = ' ';

            while (!(choice == 'y' || choice == 'n'))
            {
                scanf(" %c", &choice);

                if (!(choice == 'y' || choice == 'n'))
                {
                    printf("Erreur, recommencez\n> ");
                }
            }
        }
        else
        {
            printf("Erreur, le codebarre entré ne correpond pas a un article consigne\n");
        }
    }
}