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
    Client client;

    char title[12];

    fscanf(file, "%s", title);

    // Scanning the file until EOF
    while (!feof(file))
    {
        // Scanning each client
        fscanf(file, "%4d", &client.id);
        fscanf(file, "%s", title);

        while (strcmp(title,"EMAIL") != 0)
        {
            if (strcmp(title, "NOM") == 0)
            {
                fscanf(file, "%s", client.lastname);
            }
            else if (strcmp(title, "PRENOM") == 0)
            {
                fscanf(file, "%s", client.lastname);
            }
            else if (strcmp(title, "PT_FIDELITE") == 0)
            {
                fscanf(file, "%d", &client.points);
            }
        }

        // here, we know that we are at the end of the client characterisations
        fscanf(file, "%s", client.email);
        fscanf(file, "%s", title);
    }
    
    // pas sur comment ça foncionne :
    // list_push(this, client);

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

/* --- StockList --------------------------------------------------------------- */

typedef enum
{
    ITEM_UNDEFINED,
    ITEM_ALCOHOL,
    ITEM_FRESH_PRODUCT,
} ItemCategory;

#define ITEM_LABEL_SIZE 64

typedef struct
{
    BareCode id;
    char label[ITEM_LABEL_SIZE];
    float price;
    int reduction;  // in pourcent
    ItemCategory category;
    bool isConsigned;
} Item;

typedef List StockList;

// Creer une liste de stock en fonction d'un fichier
StockList *stocks_create(FILE *file)
{
    StockList *this = list_create();
    Item item;
    item.isConsigned = false;
    ItemCategory ic;
    
    char title[10], cat[15];

    fscanf(file, "%s", title);
    while (!feof(file))
    {
        fscanf(file, "%4d", &item.id);
        fscanf(file, "%s", title);

        while (strcmp(title, "CATEGORIE") != 0)
        {
            if (strcmp(title, "LABEL") == 0)
            {
                fscanf(file, "%s", item.label);
            }
            else if (strcmp(title, "PRIX") == 0)
            {
                fscanf(file, "%f", &item.price);
            }
            else if (strcmp(title, "REDUCTION") == 0)
            {
                fscanf(file, "%d", &item.reduction);
            }
            else if (strcmp(title, "CONSIGNE") == 0)
            {
                item.isConsigned = true;
            }
            fscanf(file, "%s", cat);
            
            // here, compare the value of cat, depending of the result, into item.category

        }

    }

    // listpush();
    return this; 

}

// Destruit la liste des stockes.
void stocks_destroy(StockList *this)
{
    // list_destroy(item);
}

// Cherche un item dans les stocks
//
// Notes: return NULL si l'objet ne peux pas etre trouver
Item *stocks_lookup_item(StockList *this, BareCode barecode)
{

}


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
bool backet_add_item(Basket *this, StockList *stocks, int barecode, int quantity);

// Affiche le ticket dans FILE
void basket_print_bill(Basket *this, FILE *file);

/* --- Entry point of the application --------------------------------------- */

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    FILE *fStock, *fClient;
    fStock = fopen("stock.dat","r");
    fClient = fopen("client.dat","r");

    ClientsList *clientsL;
    StockList *StockL;


    // Lecture client.dat
    clientsL = clients_create (fClient);

    // Lecture stock.dat
    StockL = stocks_create(fStock);


    return 0;
}
