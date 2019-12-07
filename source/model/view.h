#pragma once

#include "model/model.h"
#include "shop/users.h"

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

void model_view_title(User *user, Surface *surface, const char *title);

void model_view_status_bar(Surface *surface, ModelViewState *state, Model model, void *data);

void model_view(User *user, const char *title, Model model, void *data);
