#include <stdlib.h>
#include <string.h>

#include "model/view.h"
#include "shop/basket.h"
#include "utils/assert.h"
#include "utils/terminal.h"
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
        item->is_consigne = is_consigned;

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

typedef enum
{
    COL_BASKET_BARECODE,
    COL_BASKET_LABEL,
    COL_BASKET_CONSIGNE,
    CAL_BASKET_UNIT_PRICE,
    COL_BASKET_QUANTITY,
    COL_BASKET_REDUCTION,
    COL_BASKET_PRICE,

    __COL_BASKET_COUNT,
} BasketModelColumn;

ModelAccess basket_ModelReadAccess(Basket *basket, int row, int column, User *user)
{
    (void)basket;
    (void)row;
    (void)column;
    (void)user;

    return ACCESS_ALL;
}

ModelAccess basket_ModelWriteAccess(Basket *basket, int row, int column, User *user)
{
    (void)basket;
    (void)row;
    (void)user;

    if (column == COL_BASKET_BARECODE ||
        column == COL_BASKET_QUANTITY ||
        column == COL_BASKET_CONSIGNE)
    {
        return ACCESS_ALL;
    }
    else
    {
        return ACCESS_NONE;
    }
}

int basket_ModelRowCount(Basket *basket)
{
    return list_count(basket->items);
}

int basket_ModelRowCreate(Basket *basket)
{
    BasketItem *item = malloc(sizeof(BasketItem));

    *item = (BasketItem){0};

    list_pushback(basket->items, item);

    return list_count(basket->items) - 1;
}

void basket_ModelRowDelete(Basket *basket, int index)
{
    BasketItem *item_to_remove;
    list_peekat(basket->items, index, (void **)&item_to_remove);
    list_remove(basket->items, item_to_remove);
}

int basket_ModelColumnCount(void)
{
    return __COL_BASKET_COUNT;
}

const char *basket_ModelColumnName(int index, ModelRole role)
{
    (void)role;

    switch (index)
    {
    case COL_BASKET_BARECODE:
        return "BARECODE";

    case COL_BASKET_LABEL:
        return "LABEL";

    case COL_BASKET_CONSIGNE:
        return "CONSIGNE";

    case CAL_BASKET_UNIT_PRICE:
        return "UNIT_PRICE";

    case COL_BASKET_QUANTITY:
        return "QUANTITY";

    case COL_BASKET_REDUCTION:
        return "REDUCTION";

    case COL_BASKET_PRICE:
        return "PRICE";
    }

    ASSERT_NOT_REACHED();
}

VarianType basket_ModelColumnType(int index, ModelRole role)
{
    (void)role;

    switch (index)
    {
    case COL_BASKET_BARECODE:
        return VARIANT_INT;

    case COL_BASKET_LABEL:
        return VARIANT_STRING;

    case COL_BASKET_CONSIGNE:
        return VARIANT_INT;

    case CAL_BASKET_UNIT_PRICE:
        return VARIANT_FLOAT;

    case COL_BASKET_QUANTITY:
        return VARIANT_INT;

    case COL_BASKET_REDUCTION:
        return VARIANT_FLOAT;

    case COL_BASKET_PRICE:
        return VARIANT_FLOAT;
    }

    ASSERT_NOT_REACHED();
}

Style basket_ModelColumnStyle(int index)
{
    (void)index;

    return style_centered(DEFAULT_STYLE);
}

Variant basket_ModelGetData(Basket *basket, int row, int column, ModelRole role)
{
    (void)role;

    BasketItem *basket_item;
    list_peekat(basket->items, row, (void **)&basket_item);
    Item *item = stocks_lookup_item(basket->stocks, basket_item->barecode);

    if (item == NULL)
    {
        return vstring("(null)");
    }
    else
    {
        if (basket_item->is_consigne)
        {
            switch (column)
            {
            case COL_BASKET_BARECODE:
                return vint(basket_item->barecode);

            case COL_BASKET_LABEL:
                return vstring(item->label);

            case COL_BASKET_CONSIGNE:
                if (role == ROLE_DATA)
                {
                    return vint(1);
                }
                else
                {
                    return vstring("oui");
                }

            case CAL_BASKET_UNIT_PRICE:
                if (role == ROLE_DATA)
                {
                    return vfloat(item->consignedValue);
                }
                else
                {
                    return vstringf("%5.2f€", item->consignedValue);
                }

            case COL_BASKET_QUANTITY:
                if (role == ROLE_DATA)
                {
                    return vint(basket_item->quantity);
                }
                else
                {
                    return vstringf("x %-3d", basket_item->quantity);
                }

            case COL_BASKET_REDUCTION:
                return vstring("-");

            case COL_BASKET_PRICE:
                if (role == ROLE_DATA)
                {
                    return vfloat(-(item->consignedValue * basket_item->quantity));
                }
                else
                {
                    return vstringf("-%5.2f€", (item->consignedValue * basket_item->quantity));
                }
            }

            ASSERT_NOT_REACHED();
        }
        else
        {
            switch (column)
            {
            case COL_BASKET_BARECODE:
                return vint(basket_item->barecode);

            case COL_BASKET_LABEL:
                return vstring(item->label);

            case COL_BASKET_CONSIGNE:
                if (role == ROLE_DATA)
                {
                    return vint(0);
                }
                else
                {
                    return vstring("non");
                }

            case CAL_BASKET_UNIT_PRICE:
                return vfloat(item->price);

            case COL_BASKET_QUANTITY:
                return vint(basket_item->quantity);

            case COL_BASKET_REDUCTION:
                return vint(item->discount);

            case COL_BASKET_PRICE:
                return vfloat((item->price * basket_item->quantity) * (1 - item->discount / 100.0));
            }

            ASSERT_NOT_REACHED();
        }
    }
}

void basket_ModelSetData(Basket *basket, int row, int column, Variant value, ModelRole role)
{
    (void)role;

    BasketItem *basket_item;
    list_peekat(basket->items, row, (void **)&basket_item);

    switch (column)
    {
    case COL_BASKET_BARECODE:
        basket_item->barecode = value.as_int;
        break;
    case COL_BASKET_CONSIGNE:
        basket_item->is_consigne = value.as_int;
        break;
    case COL_BASKET_QUANTITY:
        basket_item->quantity = value.as_int;
        break;
    }
}

void add_item_BasketAction(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    Basket *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    terminal_disable_alternative_screen_buffer();

    terminal_enable_alternative_screen_buffer();
}

void add_consigned_BasketAction(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    Basket *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    surface_update(surface);

    surface_clear(surface, DEFAULT_STYLE);

    model_view_title(user, surface, "Ajouter une consigne au panier");

    surface_pop_clip(surface);

    surface_render(surface);

    terminal_read_key();
}

void remove_item_basket_action(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    Basket *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;
}

void print_bill_basket_action(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    Basket *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;
}

#define BASKET_MODEL_ACTION {'a', (ModelActionCallback)add_item_BasketAction, "Ajouter", "Ajouter un article au panier."},         \
                            {'c', (ModelActionCallback)add_consigned_BasketAction, "Consign", "Ajouter une consigne au panier."},  \
                            {'d', (ModelActionCallback)remove_item_basket_action, "Supprimer", "Supprimer un article du panier."}, \
                            {'p', (ModelActionCallback)print_bill_basket_action, "Imprimer", "Imprimer le ticket pour le client."},

ModelAction basket_actions[] = {DEFAULT_MODEL_MOVE_ACTION BASKET_MODEL_ACTION END_MODEL_VIEW_ACTION};

ModelAction *basket_ModelGetActions(void)
{
    return basket_actions;
}

Model basket_model_create(void)
{
    return (Model){
        (ModelReadAccess)basket_ModelReadAccess,
        (ModelWriteAccess)basket_ModelWriteAccess,

        (ModelRowCount)basket_ModelRowCount,
        (ModelRowCreate)basket_ModelRowCreate,
        (ModelRowDelete)basket_ModelRowDelete,

        (ModelColumnCount)basket_ModelColumnCount,
        (ModelColumnName)basket_ModelColumnName,
        (ModelColumnType)basket_ModelColumnType,
        (ModelColumnStyle)basket_ModelColumnStyle,

        (ModelGetData)basket_ModelGetData,
        (ModelSetData)basket_ModelSetData,

        (ModelGetActions)basket_ModelGetActions,
    };
}