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
} ModelViewState;

void model_view(const char *title, Model model, void *data);