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

const char *stocks_ModelColumnName(int index)
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

Variant stocks_ModelGetData(StockList *stock, int row, int column)
{
    Item *item;
    list_peekat(stock, row, (void **)&item);
    assert(item);

    switch (column)
    {
    case COL_ITEM_BARECODE:
        return vint(item->id);

    case COL_ITEM_LABEL:
        return vstring(item->label);

    case COL_ITEM_PRICE:
        return vfloat(item->price);

    case COL_ITEM_CONSIGNED:
        return vfloat(item->consignedValue);

    case COL_ITEM_DISCOUNT:
        return vint(item->discount);

    case COL_ITEM_CATEGORY:
        return vint(item->category);
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

Model stocks_model_create(void)
{
    return (Model){
        (ModelRowCount)stocks_ModelRowCount,
        (ModelRowCreate)stocks_ModelRowCreate,
        (ModelRowDelete)stocks_ModelRowDelete,

        (ModelColumnCount)stocks_ModelColumnCount,
        (ModelColumnName)stocks_ModelColumnName,
        (ModelColumnType)stocks_ModelColumnType,

        (ModelGetData)stocks_ModelGetData,
        (ModelSetData)stocks_ModelSetData,
    };
}
