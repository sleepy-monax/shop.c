#pragma once

#include <stdio.h>
#include "utils/variant.h"
#include "model/model_action.h"

typedef enum
{
    ROLE_DATA,
    ROLE_DISPLAY
} ModelRole;

typedef int (*ModelRowCount)(void *data);
typedef int (*ModelRowCreate)(void *data);
typedef void (*ModelRowDelete)(void *data, int index);

typedef int (*ModelColumnCount)(void);
typedef const char *(*ModelColumnName)(int index, ModelRole role);
typedef VarianType (*ModelColumnType)(int index);

typedef Variant (*ModelGetData)(void *data, int row, int column, ModelRole role);
typedef void (*ModelSetData)(void *data, int row, int column, Variant value);

typedef ModelAction *(*ModelGetActions)(void);

typedef struct Model
{
    ModelRowCount row_count;
    ModelRowCreate row_create;
    ModelRowDelete row_delete;

    ModelColumnCount column_count;
    ModelColumnName column_name;
    ModelColumnType column_type;

    ModelGetData get_data;
    ModelSetData set_data;

    ModelGetActions get_actions;
} Model;

int model_get_column(Model model, const char *name);

void model_load(Model model, void *data, FILE *source);

void model_save(Model model, void *data, FILE *destination);
