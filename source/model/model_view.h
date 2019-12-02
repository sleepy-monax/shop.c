#pragma once

#include <stdbool.h>
#include "model/model.h"

typedef struct ModelViewState
{
    bool exited;

    int scroll;
    int slected;

    int width;
    int height;

    int sortby;
    bool sort_accending;
    bool sort_dirty;

    // FIXME: il y a peux etre moyen de faire mieux ^^
    int sorted[10000];
} ModelViewState;

void model_view_edit(ModelViewState state, Model model, void *data, int row);

void model_view_draw_status_bar(ModelViewState state, Model model, void *data, const char *msg);

void model_view_draw_title(const char *title, int width);

void model_view(const char *title, Model model, void *data);