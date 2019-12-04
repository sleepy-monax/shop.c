#include <stdlib.h>
#include <string.h>

#include "shop/basket.h"
#define ITEM_STRING_ENTRY(__x) #__x,

// Creer un nouveau panier.
Basket *basket_create(void)
{
    return list_create();
}

void basket_destroy(Basket *basket)
{
    list_destroy(basket);
}

BasketItem *Basket_lookup(Basket *this, BareCode id)
{
    list_foreach(item, this)
    {
        BasketItem *b = (BasketItem *)item->value;

        if (b->item->id == id)
        {
            return b;
        }
    }
    return NULL;
}

// Ajoute un item dans le panier du client
void basket_add_item(Basket *this, Item *stockItem, int quantity)
{
    BasketItem *existingItem;
    existingItem = Basket_lookup(this, stockItem->id);

    if (existingItem != NULL)
        existingItem->quantity += quantity;
    else
    {
        BasketItem *item = malloc(sizeof(BasketItem));
        item->item = stockItem;
        item->quantity = quantity;

        list_pushback(this, item);
    }
}

void basket_print_bill(Basket *this)
{
    float totPrice_item, totPriceBasket = 0.;

    printf("Label                 Prix\n");
    printf("--------------------------\n");

    list_foreach(item, this)
    {
        BasketItem *b = (BasketItem *)item->value;

        totPrice_item = b->item->price;
        if (b->item->discount != 0)
            totPrice_item -= (b->item->discount / 100) * totPrice_item;
        totPrice_item *= b->quantity;

        totPriceBasket += totPrice_item;

        if (b->quantity == 1)
            printf("%-s           %-5.2f€\n", b->item->label, b->item->price);
        else
            printf("%-s  %-d x %-5.2f€    %-5.2f€\n", b->item->label, b->quantity, b->item->price, totPrice_item);
        // todo : sort per categories
    }

    printf("\nTOTAL : %5.2f€\n", totPriceBasket);
}