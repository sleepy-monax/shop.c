#include <stdlib.h>

#include "utils/input.h"
#include "utils/string.h"
#include "view/views.h"

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

void cashier_return_consigned_bottles(Session *session, StockList *stock)
{
    //TODO: Add support for session
    (void)session;

    float totValue = 0.;
    BareCode bottle_barecode;
    char bottle_raw_barecode[5];
    char bottle_raw_count[5];
    int bottle_count;
    Item *bottle;

    do
    {
        stocks_display_consigned(stock);

        user_input("Inserez le codebarre de la bouteille à rendre", "####", bottle_raw_barecode);
        bottle_barecode = atoi(bottle_raw_barecode);

        bottle = stocks_lookup_item(stock, bottle_barecode);

        if (bottle && bottle->isConsigned)
        {
            user_input("Entrez le nombre de bouteilles à rendre", "####", bottle_raw_count);
            bottle_count = atoi(bottle_raw_count);

            totValue += bottle->consignedValue * bottle_count;

            printf("Vous allez recuperer %5.2f€\n", totValue);
        }
        else
        {
            printf("Erreur, le codebarre entré ne correpond pas à un article consigne\n");
        }
    } while (user_yes_no("Voulez-vous continuer ?") == YES);
}