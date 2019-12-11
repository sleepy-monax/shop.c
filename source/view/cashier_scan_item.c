#include "utils/input.h"
#include "utils/logger.h"
#include "view/views.h"

#include <stdlib.h>
#include <time.h>

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds)
        ;
}

void cashier_scan_items(Basket *basket, StockList *stock)
{
    Item *item;

    BareCode item_barecode = -1;
    char item_raw_barecode[5];

    int item_quantity = -1;
    char item_raw_quantity[5];

    float totValue = 0.;

    do
    {
        stocks_display(stock);
        user_input("Inserez le codebarre de l'article", "####", item_raw_barecode);

        item_barecode = atoi(item_raw_barecode);
        item = stocks_lookup_item(stock, item_barecode);

        if (item != NULL)
        {
            user_input("Entrez la quatite que vous souhaitez acheter", "####", item_raw_quantity);
            item_quantity = atoi(item_raw_quantity);

            if (item_quantity <= 0)
            {
                printf("Achat annule!\n");
            }
            else
            {
                basket_add_item(basket, item_barecode, false, item_quantity);

                totValue = item->price;
                totValue -= item->price * item->discount;
                totValue *= item_quantity;
            }
        }

    } while (user_yes_no("Voulez-vous continuer a ajouter des articles au panier ?") == YES);
}