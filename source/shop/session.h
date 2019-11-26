#pragma once

#include "shop/clients.h"
#include "shop/basket.h"

typedef struct
{
    Client *client;
    Basket *basket;
} Session;

Session *session_create(void);

void session_destroy(Session *session);

void session_print_bill(Session *session);