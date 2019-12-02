#pragma once

#include <stdio.h>

#include "utils/list.h"
#include "shop/shop.h"
#include "model/model.h"

#define CLIENT_FIRST_NAME_SIZE 64
#define CLIENT_LAST_NAME_SIZE 64
#define CLIENT_EMAIL_SIZE 64

typedef struct
{
    BareCode id;

    char firstname[CLIENT_FIRST_NAME_SIZE];
    char lastname[CLIENT_LAST_NAME_SIZE];
    char email[CLIENT_EMAIL_SIZE];

    int points;
} Client;

typedef List ClientsList;

void clients_display(ClientsList *clients);

Client *clients_lookup(ClientsList *clients, BareCode id);

int clients_generate_id(ClientsList *clients);

Model clients_model_create(void);
