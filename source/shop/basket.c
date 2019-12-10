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
