#pragma once

#include "shop/stocks.h"
#include "shop/shop.h"
#include "utils/list.h"

typedef struct
{
    Item *item;
    int quantity;
} BasketItem;

typedef List Basket;

Basket *basket_create(void);

void basket_destroy(Basket *basket);

void basket_add_item(Basket *this, Item *item, int quantity);

void basket_print_bill(Basket *this);