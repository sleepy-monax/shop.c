#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "shop/clients.h"
#include "utils/assert.h"

typedef enum
{
    COL_CLIENTS_BARECODE,
    COL_CLIENTS_LASTNAME,
    COL_CLIENTS_FIRSTNAME,
    COL_CLIENTS_EMAIL,
    COL_CLIENTS_POINTS,

    __COL_CLIENTS_COUNT,
} ClientModelColumn;

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

int clients_ModelRowCount(ClientsList *clients)
{
    return list_count(clients);
}

int clients_ModelRowCreate(ClientsList *clients)
{
    Client *new_client = malloc(sizeof(Client));
    *new_client = (Client){0};
    new_client->id = clients_generate_id(clients);

    list_pushback(clients, new_client);

    return list_count(clients) - 1;
}

void clients_ModelRowDelete(ClientsList *clients, int index)
{
    Client *client_to_remove;
    list_peekat(clients, index, (void **)&client_to_remove);
    list_remove(clients, client_to_remove);
}

int clients_ModelColumnCount(void)
{
    return __COL_CLIENTS_COUNT;
}

const char *clients_ModelColumnName(int index, ModelRole role)
{
    if (role == ROLE_DATA)
    {
        switch (index)
        {
        case COL_CLIENTS_BARECODE:
            return "BARECODE";

        case COL_CLIENTS_FIRSTNAME:
            return "FIRSTNAME";

        case COL_CLIENTS_LASTNAME:
            return "LASTNAME";

        case COL_CLIENTS_EMAIL:
            return "EMAIL";

        case COL_CLIENTS_POINTS:
            return "POINTS";
        }
    }
    else
    {
        switch (index)
        {
        case COL_CLIENTS_BARECODE:
            return "Code";

        case COL_CLIENTS_FIRSTNAME:
            return "Prénom";

        case COL_CLIENTS_LASTNAME:
            return "Nom";

        case COL_CLIENTS_EMAIL:
            return "E-mail";

        case COL_CLIENTS_POINTS:
            return "Points";
        }
    }

    ASSERT_NOT_REACHED();
}

VarianType clients_ModelColumnType(int index)
{
    switch (index)
    {
    case COL_CLIENTS_BARECODE:
        return VARIANT_INT;

    case COL_CLIENTS_FIRSTNAME:
        return VARIANT_STRING;

    case COL_CLIENTS_LASTNAME:
        return VARIANT_STRING;

    case COL_CLIENTS_EMAIL:
        return VARIANT_STRING;

    case COL_CLIENTS_POINTS:
        return VARIANT_INT;
    }

    ASSERT_NOT_REACHED();
}

Style clients_ModelColumnStyle(int index)
{
    switch (index)
    {
    case COL_CLIENTS_BARECODE:
        return style_centered(DEFAULT_STYLE);

    case COL_CLIENTS_FIRSTNAME:
        return style_centered(DEFAULT_STYLE);

    case COL_CLIENTS_LASTNAME:
        return style_centered(DEFAULT_STYLE);

    case COL_CLIENTS_EMAIL:
        return DEFAULT_STYLE;

    case COL_CLIENTS_POINTS:
        return style_centered(DEFAULT_STYLE);
    }

    ASSERT_NOT_REACHED();
}

Variant clients_ModelGetData(ClientsList *clients, int row, int column, ModelRole role)
{
    (void)role;

    Client *client;
    list_peekat(clients, row, (void **)&client);

    switch (column)
    {
    case COL_CLIENTS_BARECODE:
        if (role == ROLE_DATA)
            return vint(client->id);
        else
            return vstringf("%04d", client->id);
    case COL_CLIENTS_FIRSTNAME:
        return vstring(client->firstname);

    case COL_CLIENTS_LASTNAME:
        return vstring(client->lastname);

    case COL_CLIENTS_EMAIL:
        return vstring(client->email);

    case COL_CLIENTS_POINTS:
        if (role == ROLE_DATA)
        {
            return vint(client->points);
        }
        else
        {
            return vstringf("%4d", client->points);
        }
    }

    ASSERT_NOT_REACHED();
}

void clients_ModelSetData(ClientsList *clients, int row, int column, Variant value)
{
    Client *client;
    list_peekat(clients, row, (void **)&client);
    assert(client);

    switch (column)
    {
    case COL_CLIENTS_BARECODE:
        client->id = value.as_int;
        break;

    case COL_CLIENTS_FIRSTNAME:
        strcpy(client->firstname, value.as_string);
        break;

    case COL_CLIENTS_LASTNAME:
        strcpy(client->lastname, value.as_string);
        break;

    case COL_CLIENTS_EMAIL:
        strcpy(client->email, value.as_string);
        break;

    case COL_CLIENTS_POINTS:
        client->points = value.as_int;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
}

ModelAction clients_actions[] = {DEFAULT_MODEL_VIEW_ACTION END_MODEL_VIEW_ACTION};

ModelAction *clients_ModelGetActions(void)
{
    return clients_actions;
}

Model clients_model_create(void)
{
    return (Model){
        (ModelRowCount)clients_ModelRowCount,
        (ModelRowCreate)clients_ModelRowCreate,
        (ModelRowDelete)clients_ModelRowDelete,

        (ModelColumnCount)clients_ModelColumnCount,
        (ModelColumnName)clients_ModelColumnName,
        (ModelColumnType)clients_ModelColumnType,
        (ModelColumnStyle)clients_ModelColumnStyle,

        (ModelGetData)clients_ModelGetData,
        (ModelSetData)clients_ModelSetData,

        (ModelGetActions)clients_ModelGetActions,
    };
}
