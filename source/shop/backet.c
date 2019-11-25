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
void backet_add_item(Basket *this, Item *stockItem, int quantity)
{
    BasketItem *item = malloc(sizeof(BasketItem));

    item->item = stockItem;
    item->quantity = quantity;

    list_pushback(this, item);
}

void basket_print_bill(Basket *this)
{
    (void)this;
    //TODO print the ticket.
}