#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <assert.h>

#include "shop/stocks.h"

#define ITEM_STRING_ENTRY(__x) #__x,
const char *item_category_string[] = {ITEM_CATEGORY_LIST(ITEM_STRING_ENTRY) NULL};

// Creer une liste de stock en fonction d'un fichier
StockList *stocks_create(FILE *file)
{
    assert(file != NULL);

    StockList *this = list_create();

    char title[10];

    fscanf(file, "%s", title);
    while (!feof(file))
    {
        assert(strcmp(title, "ITEM") == 0);
        Item *item = malloc(sizeof(Item));
        *item = (Item){};
        fscanf(file, "%4d", &item->id);
        fscanf(file, "%s", title);

        while (strcmp(title, "ITEM") != 0 && !feof(file))
        {
            fflush(stdout);
            if (strcmp(title, "LABEL") == 0)
            {
                fscanf(file, "%s", item->label);
            }
            else if (strcmp(title, "PRIX") == 0)
            {
                fscanf(file, "%f", &item->price);
            }
            else if (strcmp(title, "REDUCTION") == 0)
            {
                fscanf(file, "%d", &item->reduction);
            }
            else if (strcmp(title, "CONSIGNE") == 0)
            {
                item->isConsigned = true;
                fscanf(file, "%f", &item->consignedValue);
            }
            else if (strcmp(title, "CATEGORIE") == 0)
            {
                char categorie[64];
                fscanf(file, "%s", categorie);

                for (int i = 0; item_category_string[i] != NULL; i++)
                {
                    if (strcmp(categorie, item_category_string[i]) == 0)
                    {
                        item->category = (ItemCategory)i;
                    }
                }
            }

            fscanf(file, "%s", title);
        }

        list_pushback(this, item);
    }

    return this;
}

void stocks_display(StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;
        barecode_print(itemInStocks->id);

        printf(" %04d %s %s", itemInStocks->id, itemInStocks->label, item_category_string[itemInStocks->category]);

        if (itemInStocks->reduction != 0)
        {
            printf(" \e[103;30m-%d%%\e[0m", itemInStocks->reduction);
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