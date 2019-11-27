#include <stdlib.h>

#include "shop/basket.h"

// Creer un nouveau panier.
Basket *basket_create(void)
{
    return list_create();
}

void basket_destroy(Basket *basket)
{
    list_destroy(basket);
}

// Ajoute un item dans le panier du client
void basket_add_item(Basket *this, Item *stockItem, int quantity)
{
    BasketItem *item = malloc(sizeof(BasketItem));

    item->item = stockItem;
    item->quantity = quantity;

    list_pushback(this, item);
}

void basket_print_bill(Basket *this)
{
    float totPrice_item;
    list_foreach(item, this)
    {
        BasketItem *item = (BasketItem *)item->item;

        totPrice_item = item->item->price;
        if (item->item->reduction != 0) totPrice_item -= item->item->reduction * item->item->price;
        totPrice_item *= item->quantity;

        printf("%4d %-s %d %5.2f\n", item->item->id, item->item->label, item->quantity, totPrice_item);
    }
}