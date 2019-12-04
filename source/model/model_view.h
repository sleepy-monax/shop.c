#pragma once

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

void model_view(const char *title, Model model, void *data);
