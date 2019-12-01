#pragma once

#include <stdbool.h>
#include "model/model.h"

typedef struct
{
    bool exited;

    int scroll;
    int slected;

    int width;
    int height;

    int sortby;
    bool sort_accending;

    // FIXME: il y a peux etre moyen de faire mieux ^^
    int sorted[10000];
} ModelViewState;

void model_view(const char *title, Model model, void *data);