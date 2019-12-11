#include <stdlib.h>
#include <string.h>

#include "shop/basket.h"
#include "utils/math.h"

Basket *basket_create(StockList *stocks, Client *owner)
{
    Basket *this = malloc(sizeof(Basket));

    this->pay_with_point = false;
    this->items = list_create();
    this->stocks = stocks;
    this->owner = owner;

    return this;
}

void basket_destroy(Basket *this)
{
    list_destroy(this->items);
    free(this);
}

BasketItem *Basket_lookup(Basket *this, BareCode id, bool is_consigned)
{
    list_foreach(item, this->items)
    {
        BasketItem *b = (BasketItem *)item->value;

        if (b->barecode == id && b->is_consigne == is_consigned)
        {
            return b;
        }
    }
    return NULL;
}

void basket_add_item(Basket *this, BareCode barecode, bool is_consigned, int quantity)
{
    BasketItem *existingItem;
    existingItem = Basket_lookup(this, barecode, is_consigned);

    if (existingItem != NULL)
        existingItem->quantity += quantity;
    else
    {
        BasketItem *item = malloc(sizeof(BasketItem));

        item->barecode = barecode;
        item->quantity = quantity;
        item->is_consigne = true;

        list_pushback(this->items, item);
    }
}

float basket_bill(Basket *this, FILE *fout)
{
    if (fout)
    {
        fprintf(fout, "Voici le contenu du panier : \n\n");
        fprintf(fout, "Label                     Prix            Prix discount\n");
        fprintf(fout, "-------------------------------------------------------\n");
    }

    float basket_total = 0;
    float basket_discount = 0;

    list_foreach(item, this->items)
    {
        BasketItem *basket_item = (BasketItem *)item->value;
        Item *stock_item = stocks_lookup_item(this->stocks, basket_item->barecode);

        if (fout)
        {
            if (basket_item->quantity)
            {
                fprintf(fout, "%04d %s x%-3d ", basket_item->barecode, stock_item->label, basket_item->quantity);
            }
            else
            {
                fprintf(fout, "%04d %s ", basket_item->barecode, stock_item->label);
            }
        }

        float basket_item_price;
        float basket_item_discount;

        if (basket_item->is_consigne)
        {
            basket_item_price = -(stock_item->consignedValue * basket_item->quantity);
            basket_item_discount = 0;

            if (fout)
            {
                fprintf(fout, "consigne %4.2f ", stock_item->consignedValue);
            }
        }
        else if (stock_item->discount)
        {
            basket_item_discount = stock_item->price * basket_item->quantity * (stock_item->discount / 100.0);
            basket_item_price = stock_item->price * basket_item->quantity - basket_discount;

            if (fout)
            {
                fprintf(fout, "-%-3d%% ", stock_item->discount);
            }
        }
        else
        {
            basket_item_price = stock_item->price * basket_item->quantity;
            basket_item_discount = 0;
        }

        if (fout)
        {
            fprintf(fout, "%7.2f€\n", basket_item_price);
        }

        basket_total += basket_item_price;
        basket_discount += basket_item_discount;
    }

    if (this->owner && this->pay_with_point)
    {
        int point_used = min(basket_total * 100, this->owner->points);

        if (fout)
        {
            fprintf(fout, "Reduction fidelité: %.2f€ (%dpts)\n", point_used / 100.0, point_used);
        }

        basket_total -= (point_used / 100.0);
    }

    if (fout)
    {
        if (basket_discount && fout)
        {
            fprintf(fout, "Reduction %5.2f€\n", basket_discount);
        }

        fprintf(fout, "TOTAL : %5.2f€\n", basket_total);

        if (this->owner)
        {
            fprintf(fout, "Vous avez gagnez %dpts\n", (int)basket_total / 10);
        }
    }

    return basket_total;
}
