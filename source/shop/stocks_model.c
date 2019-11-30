#include "shop/stocks.h"
#include "model/model.h"
#include "utils/assert.h"

typedef enum
{
    COL_ITEM_BARECODE,
    COL_ITEM_LABEL,
    COL_ITEM_PRICE,
    COL_ITEM_CONSIGNED,
    COL_ITEM_REDUCTION,
    COL_ITEM_CATEGORY,

    __COL_ITEM_COUNT,
} StockModelColumn;

int stocks_ModelRowCount(StockList *stock)
{
    return list_count(stock);
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

    case COL_ITEM_REDUCTION:
        return "REDUCTION";

    case COL_ITEM_CATEGORY:
        return "CATEGORY";
    }

    ASSERT_NOT_REACHED();
}

Variant stocks_ModelData(StockList *stock, int row, int column)
{
    Item *item;
    list_peekat(stock, row, (void **)&item);

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

    case COL_ITEM_REDUCTION:
        return vint(item->reduction);

    case COL_ITEM_CATEGORY:
        return vint(item->category);
    }

    ASSERT_NOT_REACHED();
}

Model stocks_model_create(void)
{
    return (Model){
        (ModelRowCount)stocks_ModelRowCount,
        (ModelColumnCount)stocks_ModelColumnCount,
        (ModelColumnName)stocks_ModelColumnName,
        (ModelData)stocks_ModelData,
    };
}
