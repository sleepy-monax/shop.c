#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "shop/stocks.h"

#define ITEM_STRING_ENTRY(__x) #__x,
const char *item_category_string[] = {ITEM_CATEGORY_LIST(ITEM_STRING_ENTRY) NULL};

void stocks_display(StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;
        barecode_print(itemInStocks->id);

        printf(" %04d %s %s %5.2f€", itemInStocks->id, itemInStocks->label, item_category_string[itemInStocks->category], itemInStocks->price);

        if (itemInStocks->discount != 0)
        {
            printf(" \e[103;30m-%d%%\e[0m", itemInStocks->discount);
        }

        printf("\n");
    }
}

void stocks_display_consigned(StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;
        if (itemInStocks->isConsigned)
        {
            printf("%04d %s %s\n", itemInStocks->id, itemInStocks->label, item_category_string[itemInStocks->category]);
        }
    }
}

// Destruit la liste des stockes.
void stocks_destroy(StockList *this)
{
    list_destroy(this);
}

// Cherche un item dans les stocks
//
// Notes: return NULL si l'objet ne peux pas etre trouver
Item *stocks_lookup_item(StockList *stocks, BareCode barecode)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;

        if (itemInStocks->id == barecode)
        {
            return itemInStocks;
        }
    }

    return NULL;
}

int stocks_generate_id(StockList *stocks)
{

    int id;

    srand(time(NULL));

    do
    {
        id = rand() % 9999;
        if (list_count(stocks) == 0)
            return id;
    } while (!stocks_lookup_item(stocks, id));

    return id;
}