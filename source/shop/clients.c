#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "shop/clients.h"

ClientsList *clients_create(FILE *file)
{
    assert(file != NULL);

    ClientsList *this = list_create();

    char title[64];

    fscanf(file, "%s", title);
    // Scanning the file until EOF
    while (!feof(file))
    {
        assert(strcmp(title, "CLIENT") == 0);
        Client *client = (Client *)malloc(sizeof(Client));
        *client = (Client){};

        // Scanning each client
        fscanf(file, "%4d", &client->id);
        fscanf(file, "%s", title);

        while (strcmp(title, "CLIENT") != 0 && !feof(file))
        {
            if (strcmp(title, "NOM") == 0)
            {
                fscanf(file, "%s", client->lastname);
            }
            else if (strcmp(title, "PRENOM") == 0)
            {
                fscanf(file, "%s", client->firstname);
            }
            else if (strcmp(title, "PT_FIDELITE") == 0)
            {
                fscanf(file, "%d", &client->points);
            }
            else if (strcmp(title, "EMAIL") == 0)
            {
                fscanf(file, "%s", client->email);
            }

            fscanf(file, "%s", title);
        }

        list_pushback(this, client);
    }

    return this;
}

void clients_display(ClientsList *clients)
{
    list_foreach(item, clients)
    {
        Client *client = (Client *)item->value;
        printf("%04d %s %s\n", client->id, client->firstname, client->lastname);
    }
}

void clients_sync(ClientsList *clients, FILE *file)
{
    (void)clients;
    (void)file;
}

Client *client_lookup(ClientsList *clients, BareCode id)
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

void clients_destroy(ClientsList *clients)
{
    list_destroy(clients);
}