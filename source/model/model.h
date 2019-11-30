#pragma once

#include "utils/variant.h"

typedef int (*ModelRowCount)(void *data);

typedef int (*ModelColumnCount)(void);

typedef const char *(*ModelColumnName)(int index);

typedef Variant (*ModelData)(void *data, int row, int column);

typedef struct
{
    ModelRowCount row_count;
    ModelColumnCount column_count;
    ModelColumnName column_name;
    ModelData data;
} Model;
