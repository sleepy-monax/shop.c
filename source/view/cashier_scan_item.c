#include <stdlib.h>
#include <string.h>

#include "utils/input.h"
#include "utils/logger.h"
#include "view/views.h"

void cashier_scan_items(Basket *basket, StockList *stock)
{
    Item *item;

    BareCode item_barecode = -1;
    char item_raw_barecode[5];

    int item_quantity = -1;
    char item_raw_quantity[5];

    bool exited = false;

    do
    {
        stocks_display(stock);
        user_input("Inserez le codebarre de l'article", "####", item_raw_barecode);

        if (strlen(item_raw_barecode) == 0)
        {
            if (user_yes_no("Voulez-vous continuer a ajouter des articles au panier ?", YES) == NO)
            {
                exited = true;
            }
        }
        else
        {
            item_barecode = atoi(item_raw_barecode);
            item = stocks_lookup_item(stock, item_barecode);

            if (item == NULL)
            {
                log_error("Code bare %04d incorrect!", item_barecode);
                continue;
            }

            user_input("Entrez la quatite que vous souhaitez acheter", "####", item_raw_quantity);
            item_quantity = atoi(item_raw_quantity);

            if (item_quantity == 0)
            {
                log_warn("Achat annulé!");
                continue;
            }

            BasketItem *item_in_basket = Basket_lookup(basket, item_barecode, false);
            int total_asked_quantity = (item_in_basket->quantity + item_quantity);

            if (total_asked_quantity > item->quantity)
            {
                log_error("Stock insufisants (%d en stock mais %d demander)", item->quantity, total_asked_quantity);
                continue;
            }

            basket_add_item(basket, item_barecode, false, item_quantity);
        }
    } while (!exited);
}