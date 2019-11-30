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

    printf("Qtt.  Label         Prix\n");
    printf("------------------------\n");
    
    list_foreach(item, this)
    {
        BasketItem *b = (BasketItem *)item->value;

        totPrice_item = b->item->price;
        if (b->item->reduction != 0)
            totPrice_item -= (b->item->reduction / 100) * totPrice_item;
        totPrice_item *= b->quantity;

        printf("%d    %-s  %5.2f€\n", b->quantity, b->item->label, b->item->price);
        // todo : sort per categories
    }
}