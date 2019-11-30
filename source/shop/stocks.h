#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "shop/shop.h"
#include "utils/list.h"
#include "model/model.h"

#define ITEM_CATEGORY_LIST(__ENTRY) \
    __ENTRY(UNDEFINED)              \
    __ENTRY(OTHER)                  \
    __ENTRY(ALCOHOL)                \
    __ENTRY(FRESH_PRODUCT)          \
    __ENTRY(COMPUTER)               \
    __ENTRY(ELECTRONIC)             \
    __ENTRY(TOYS)

#define ITEM_ENUM_ENTRY(__x) __ITEM_##__x,

typedef enum
{
    ITEM_CATEGORY_LIST(ITEM_ENUM_ENTRY)
} ItemCategory;

#define ITEM_LABEL_SIZE 64

typedef struct
{
    BareCode id;
    char label[ITEM_LABEL_SIZE];

    float price;
    int discount; // in pourcent
    ItemCategory category;
    bool isConsigned;
    float consignedValue;
} Item;

typedef List StockList;

void stocks_display(StockList *stocks);

void stocks_display_consigned(StockList *stocks);

Item *stocks_lookup_item(StockList *stocks, BareCode barecode);

Model stocks_model_create(void);

int stocks_generate_id(StockList *stocks);