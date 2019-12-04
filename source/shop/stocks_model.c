#include <stdlib.h>
#include <string.h>

#include "shop/stocks.h"
#include "model/model.h"
#include "utils/assert.h"
#include "utils/logger.h"

typedef enum
{
    COL_ITEM_BARECODE,
    COL_ITEM_LABEL,
    COL_ITEM_PRICE,
    COL_ITEM_CONSIGNED,
    COL_ITEM_DISCOUNT,
    COL_ITEM_CATEGORY,

    __COL_ITEM_COUNT,
} StockModelColumn;

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
            return "Code";

        case COL_ITEM_LABEL:
            return "Libellé";

        case COL_ITEM_PRICE:
            return "Prix";

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

VarianType stocks_ModelColumnType(int index)
{
    switch (index)
    {
    case COL_ITEM_BARECODE:
        return VARIANT_INT;

    case COL_ITEM_LABEL:
        return VARIANT_STRING;

    case COL_ITEM_PRICE:
        return VARIANT_FLOAT;

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

    case COL_ITEM_CONSIGNED:
        return style_centered(DEFAULT_STYLE);

    case COL_ITEM_DISCOUNT:
        return style_centered(BLUE_STYLE);

    case COL_ITEM_CATEGORY:
        return style_centered(DEFAULT_STYLE);
    }

    ASSERT_NOT_REACHED();
}

#define ITEM_STRING_ENTRY(__x) #__x,
static const char *item_category_string[] = {ITEM_CATEGORY_LIST(ITEM_STRING_ENTRY) NULL};

Variant stocks_ModelGetData(StockList *stock, int row, int column, ModelRole role)
{
    Item *item;
    list_peekat(stock, row, (void **)&item);
    assert(item);

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

void stocks_ModelSetData(StockList *stock, int row, int column, Variant value)
{
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
