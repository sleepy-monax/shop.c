#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "shop/clients.h"

int clients_generate_id(ClientsList *clients)
{
    int id;

    srand(time(NULL));

    do
    {
        id = rand() % 9999;
        if (list_count(clients) == 0)
            return id;
    } while (clients_lookup(clients, id));

    return id;
}

void clients_display(ClientsList *clients)
{
    list_foreach(item, clients)
    {
        Client *client = (Client *)item->value;
        printf("%04d %s %s\n", client->id, client->firstname, client->lastname);
    }
}

Client *clients_lookup(ClientsList *clients, BareCode id)
{
    list_foreach(item, clients)
    {
        Client *client = (Client *)item->value;

        if (client->id == id)
        {
            return client;
        }
    }

    return NULL;
}