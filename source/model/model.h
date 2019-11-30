#pragma once

#include <stdio.h>
#include "utils/variant.h"

typedef int (*ModelRowCount)(void *data);
typedef int (*ModelRowCreate)(void *data);
typedef void (*ModelRowDelete)(void *data, int index);

typedef int (*ModelColumnCount)(void);
typedef const char *(*ModelColumnName)(int index);
typedef VarianType (*ModelColumnType)(int index);

typedef Variant (*ModelGetData)(void *data, int row, int column);
typedef void (*ModelSetData)(void *data, int row, int column, Variant value);

typedef struct
{
    ModelRowCount row_count;
    ModelRowCreate row_create;
    ModelRowDelete row_delete;

    ModelColumnCount column_count;
    ModelColumnName column_name;
    ModelColumnType column_type;

    ModelGetData get_data;
    ModelSetData set_data;
} Model;

int model_get_column(Model model, const char *name);

void model_load(Model model, void *data, FILE *source);

void model_save(Model model, void *data, FILE *destination);
