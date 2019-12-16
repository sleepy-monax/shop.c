#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "model/model.h"
#include "shop/stocks.h"
#include "utils/assert.h"
#include "utils/logger.h"

typedef enum
{
    COL_ITEM_BARECODE,
    COL_ITEM_LABEL,
    COL_ITEM_PRICE,
    COL_ITEM_QUANTITY,
    COL_ITEM_CONSIGNED,
    COL_ITEM_DISCOUNT,
    COL_ITEM_CATEGORY,

    __COL_ITEM_COUNT,
} StockModelColumn;

#define ITEM_STRING_ENTRY(__x) #__x,
static const char *item_category_string[] = {ITEM_CATEGORY_LIST(ITEM_STRING_ENTRY) NULL};

void stocks_display(StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;
        barecode_print(itemInStocks->id);

        printf(" %04d %16s %16s %5.2f€", itemInStocks->id, itemInStocks->label, item_category_string[itemInStocks->category], itemInStocks->price);

        if (itemInStocks->discount != 0)
        {
            printf(" \e[103;30m-%d%%\e[0m", itemInStocks->discount);
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

int stocks_generate_id(StockList *stocks)
{

    int id;

    srand(time(NULL));

    do
    {
        id = rand() % 9999;

        if (list_count(stocks) == 0)
            return id;
    } while (stocks_lookup_item(stocks, id));

    return id;
}

ModelAccess stocks_ModelReadAccess(StockList *stocks, int row, int column, User *user)
{
    (void)stocks;
    (void)row;
    (void)column;
    (void)user;

    return ACCESS_ALL;
}

ModelAccess stocks_ModelWriteAccess(StockList *stocks, int row, int column, User *user)
{
    (void)stocks;
    (void)row;
    (void)user;

    if (column == COL_ITEM_BARECODE)
    {
        return ACCESS_ADMIN;
    }
    else
    {
        return ACCESS_MANAGER;
    }
}

int stocks_ModelRowCount(StockList *stock)
{
    return list_count(stock);
}

int stocks_ModelRowCreate(StockList *stocks)
{
    Item *new_item = malloc(sizeof(Item));
    *new_item = (Item){0};
    new_item->id = stocks_generate_id(stocks);

    list_pushback(stocks, new_item);

    return list_count(stocks) - 1;
}

void stocks_ModelRowDelete(StockList *stocks, int index)
{
    Item *item_to_remove;
    list_peekat(stocks, index, (void **)&item_to_remove);
    list_remove(stocks, item_to_remove);
}

int stocks_ModelColumnCount(void)
{
    return __COL_ITEM_COUNT;
}

const char *stocks_ModelColumnName(int index, ModelRole role)
{
    if (role == ROLE_DATA)
    {
        switch (index)
        {
        case COL_ITEM_BARECODE:
            return "BARECODE";

        case COL_ITEM_LABEL:
            return "LABEL";

        case COL_ITEM_PRICE:
            return "PRICE";

        case COL_ITEM_QUANTITY:
            return "QUANTITY";

        case COL_ITEM_CONSIGNED:
            return "CONSIGNED";

        case COL_ITEM_DISCOUNT:
            return "DISCOUNT";

        case COL_ITEM_CATEGORY:
            return "CATEGORY";
        }
    }
    else
    {
        switch (index)
        {
        case COL_ITEM_BARECODE:
            return "N°.art";

        case COL_ITEM_LABEL:
            return "Dénomination";

        case COL_ITEM_PRICE:
            return "Prix";

        case COL_ITEM_QUANTITY:
            return "Quantitée";

        case COL_ITEM_CONSIGNED:
            return "Consigne";

        case COL_ITEM_DISCOUNT:
            return "Réduction";

        case COL_ITEM_CATEGORY:
            return "Section";
        }
    }

    ASSERT_NOT_REACHED();
}

VarianType stocks_ModelColumnType(int index, ModelRole role)
{
    (void)role;

    switch (index)
    {
    case COL_ITEM_BARECODE:
        return VARIANT_INT;

    case COL_ITEM_LABEL:
        return VARIANT_STRING;

    case COL_ITEM_PRICE:
        return VARIANT_FLOAT;

    case COL_ITEM_QUANTITY:
        return VARIANT_INT;

    case COL_ITEM_CONSIGNED:
        return VARIANT_FLOAT;

    case COL_ITEM_DISCOUNT:
        return VARIANT_INT;

    case COL_ITEM_CATEGORY:
        return VARIANT_INT;
    }

    ASSERT_NOT_REACHED();
}

Style stocks_ModelColumnStyle(int index)
{
    switch (index)
    {
    case COL_ITEM_BARECODE:
        return style_centered(DEFAULT_STYLE);

    case COL_ITEM_LABEL:
        return DEFAULT_STYLE;

    case COL_ITEM_PRICE:
        return style_centered(DEFAULT_STYLE);

    case COL_ITEM_QUANTITY:
        return style_centered(RED_STYLE);

    case COL_ITEM_CONSIGNED:
        return style_centered(DEFAULT_STYLE);

    case COL_ITEM_DISCOUNT:
        return style_centered(BLUE_STYLE);

    case COL_ITEM_CATEGORY:
        return style_centered(DEFAULT_STYLE);
    }

    ASSERT_NOT_REACHED();
}

Variant stocks_ModelGetData(StockList *stock, int row, int column, ModelRole role)
{
    Item *item;
    assert(list_peekat(stock, row, (void **)&item));

    switch (column)
    {
    case COL_ITEM_BARECODE:
        if (role == ROLE_DATA)
        {
            return vint(item->id);
        }
        else
        {
            return vstringf("%04d", item->id);
        }

    case COL_ITEM_LABEL:
        return vstring(item->label);

    case COL_ITEM_PRICE:
        if (role == ROLE_DATA)
        {
            return vfloat(item->price);
        }
        else
        {
            return vstringf("%5.2f€", item->price);
        }

    case COL_ITEM_QUANTITY:
        if (role == ROLE_DATA)
        {
            return vint(item->quantity);
        }
        else
        {
            if (item->quantity)
            {
                return vstringf("x%-3d", item->quantity);
            }
            else
            {
                return vstring("VIDE!");
            }
        }

    case COL_ITEM_CONSIGNED:
        if (role == ROLE_DATA)
        {
            return vfloat(item->consignedValue);
        }
        else
        {
            if (item->consignedValue == 0)
            {
                return vstring("   -  ");
            }
            else
            {
                return vstringf("%5.2f€", item->consignedValue);
            }
        }

    case COL_ITEM_DISCOUNT:
        if (role == ROLE_DATA)
        {
            return vint(item->discount);
        }
        else
        {
            if (item->discount != 0)
            {
                return vstringf("%3d%%", -item->discount);
            }

            return vstring("");
        }

    case COL_ITEM_CATEGORY:
    {
        if (role == ROLE_DATA)
        {
            return vint(item->category);
        }
        else
        {
            return vstring(item_category_string[item->category]);
        }
    }
    }

    ASSERT_NOT_REACHED();
}

void stocks_ModelSetData(StockList *stock, int row, int column, Variant value, ModelRole role)
{
    (void)role;

    Item *item;
    list_peekat(stock, row, (void **)&item);
    assert(item);

    switch (column)
    {
    case COL_ITEM_BARECODE:
        item->id = value.as_int;
        break;

    case COL_ITEM_LABEL:
        strcpy(item->label, value.as_string);
        break;

    case COL_ITEM_PRICE:
        item->price = value.as_float;
        break;

    case COL_ITEM_QUANTITY:
        item->quantity = value.as_int;
        break;

    case COL_ITEM_CONSIGNED:
        item->consignedValue = value.as_float;
        if (item->consignedValue > 0)
            item->isConsigned = true;
        break;

    case COL_ITEM_DISCOUNT:
        item->discount = value.as_int;
        break;

    case COL_ITEM_CATEGORY:
        item->category = value.as_int;
        break;

    default:
        ASSERT_NOT_REACHED();
    }
}

ModelAction stocks_actions[] = {DEFAULT_MODEL_VIEW_ACTION END_MODEL_VIEW_ACTION};

ModelAction *stocks_ModelGetActions(void)
{
    return stocks_actions;
}

Model stocks_model_create(void)
{
    return (Model){
        (ModelReadAccess)stocks_ModelReadAccess,
        (ModelWriteAccess)stocks_ModelWriteAccess,

        (ModelRowCount)stocks_ModelRowCount,
        (ModelRowCreate)stocks_ModelRowCreate,
        (ModelRowDelete)stocks_ModelRowDelete,

        (ModelColumnCount)stocks_ModelColumnCount,
        (ModelColumnName)stocks_ModelColumnName,
        (ModelColumnType)stocks_ModelColumnType,
        (ModelColumnStyle)stocks_ModelColumnStyle,

        (ModelGetData)stocks_ModelGetData,
        (ModelSetData)stocks_ModelSetData,
        (ModelGetActions)stocks_ModelGetActions,
    };
}
