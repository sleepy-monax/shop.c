#pragma once

#include <stdio.h>

#include "utils/list.h"
#include "shop/shop.h"

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

ClientsList *clients_create(FILE *file);

void clients_display(ClientsList *clients);

void clients_sync(ClientsList *clients, FILE *file);

Client *client_lookup(ClientsList *clients, BareCode id);

void clients_destroy(ClientsList *clients);
