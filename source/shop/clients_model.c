#include "shop/clients.h"
#include "utils/assert.h"

typedef enum
{
    COL_CLIENTS_BARECODE,
    COL_CLIENTS_FIRSTNAME,
    COL_CLIENTS_LASTNAME,
    COL_CLIENTS_EMAIL,
    COL_CLIENTS_POINTS,

    __COL_CLIENTS_COUNT,
} ClientModelColumn;

int clients_ModelRowCount(ClientsList *clients)
{
    return list_count(clients);
}

int clients_ModelColumnCount(void)
{
    return __COL_CLIENTS_COUNT;
}

const char *clients_ModelColumnName(int index)
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

    ASSERT_NOT_REACHED();
}

Variant clients_ModelData(ClientsList *clients, int row, int column)
{
    Client *client;
    list_peekat(clients, row, (void **)&client);

    switch (column)
    {
    case COL_CLIENTS_BARECODE:
        return vint(client->id);
    case COL_CLIENTS_FIRSTNAME:
        return vstring(client->firstname);
    case COL_CLIENTS_LASTNAME:
        return vstring(client->lastname);
    case COL_CLIENTS_EMAIL:
        return vstring(client->email);
    case COL_CLIENTS_POINTS:
        return vint(client->points);
    }

    ASSERT_NOT_REACHED();
}

Model clients_model_create(void)
{
    return (Model){
        (ModelRowCount)clients_ModelRowCount,
        (ModelColumnCount)clients_ModelColumnCount,
        (ModelColumnName)clients_ModelColumnName,
        (ModelData)clients_ModelData,
    };
}
