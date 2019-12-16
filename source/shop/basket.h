#pragma once

#include "shop/barecode.h"
#include "shop/clients.h"
#include "shop/stocks.h"
#include "utils/list.h"

typedef struct
{
    BareCode barecode;
    int quantity;
    bool is_consigne;
} BasketItem;

typedef struct
{
    bool pay_with_point;
    StockList *stocks;
    List *items;
    Client *owner;
} Basket;

Basket *basket_create(StockList *stocks, Client *owner);

void basket_destroy(Basket *basket);

void basket_add_item(Basket *this, BareCode barecode, bool is_consigned, int quantity);

Model basket_model_create(void);

float basket_pay(User *user, Basket *this, FILE *fout);

BasketItem *Basket_lookup(Basket *this, BareCode id, bool is_consigned);
