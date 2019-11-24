#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef int BareCode;

/* --- ClientsList -------------------------------------------------------------- */

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

ClientsList *clients_create(FILE *file)
{
    ClientsList *this = list_create();

    // TODO: load clients from file...

    return this;
}

void clients_sync(ClientsList *clients, FILE *file)
{
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

/* --- Stocks --------------------------------------------------------------- */

#define ITEM_CATEGORY_LIST(__ENTRY) \
    __ENTRY(UNDEFINED)              \
    __ENTRY(OTHER)                  \
    __ENTRY(ALCOHOL)                \
    __ENTRY(FRESH_PRODUCT)          \
    __ENTRY(COMPUTER)               \
    __ENTRY(ELECTRONIC)             \
    __ENTRY(TOYS)

#define ITEM_ENUM_ENTRY(__x) __ITEM_##__x,

typedef enum
{
    ITEM_CATEGORY_LIST(ITEM_ENUM_ENTRY)
} ItemCategory;

#define ITEM_STRING_ENTRY(__x) #__x,
const char *item_category_string[] = {ITEM_CATEGORY_LIST(ITEM_STRING_ENTRY)};

#define ITEM_LABEL_SIZE 64

typedef struct
{
    BareCode id;
    char label[ITEM_LABEL_SIZE];
    ItemCategory category;
} Item;

typedef List Stocks;

// Creer une liste de stock en fonction d'un fichier
Stocks *stocks_create(FILE *file);

// Destruit la liste des stockes.
void stocks_destroy(Stocks *this)
{
    list_destroy(this);
}

// Cherche un item dans les stocks
//
// Notes: return NULL si l'objet ne peux pas etre trouver
Item *stocks_lookup_item(Stocks *this, BareCode barecode);

/* --- Basket --------------------------------------------------------------- */

typedef struct
{
    BareCode id;
    int quantity;
} BasketItem;

typedef List Basket;

// Creer un nouveau panier.
Basket *basket_create()
{
    return list_create();
}

// Detruit le panier de l'utilisateur.
Basket *basket_destroy(Basket *basket);

// Ajoute un item dans le panier du client
bool backet_add_item(Basket *this, Stocks *stocks, int barecode, int quantity);

// Affiche le ticket dans FILE
void basket_print_bill(Basket *this, FILE *file);

/* --- Entry point of the application --------------------------------------- */

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    return 0;
}
