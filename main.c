#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

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

/* --- StockList --------------------------------------------------------------- */

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
const char *item_category_string[] = {ITEM_CATEGORY_LIST(ITEM_STRING_ENTRY) NULL};

#define ITEM_LABEL_SIZE 64

typedef struct
{
    BareCode id;
    char label[ITEM_LABEL_SIZE];
    float price, consignedValue;
    int reduction; // in pourcent
    ItemCategory category;
    bool isConsigned;
} Item;

typedef List StockList;

// Creer une liste de stock en fonction d'un fichier
StockList *stocks_create(FILE *file)
{
    StockList *this = list_create();

    char title[10];

    fscanf(file, "%s", title);
    while (!feof(file))
    {
        assert(strcmp(title, "ITEM") == 0);
        Item *item = malloc(sizeof(Item));
        *item = (Item){};
        fscanf(file, "%4d", &item->id);
        fscanf(file, "%s", title);

        while (strcmp(title, "ITEM") != 0 && !feof(file))
        {
            fflush(stdout);
            if (strcmp(title, "LABEL") == 0)
            {
                fscanf(file, "%s", item->label);
            }
            else if (strcmp(title, "PRIX") == 0)
            {
                fscanf(file, "%f", &item->price);
            }
            else if (strcmp(title, "REDUCTION") == 0)
            {
                fscanf(file, "%d", &item->reduction);
            }
            else if (strcmp(title, "CONSIGNE") == 0)
            {
                item->isConsigned = true;
                fscanf(file, "%f", &item->consignedValue);
            }
            else if (strcmp(title, "CATEGORIE") == 0)
            {
                char categorie[64];
                fscanf(file, "%s", categorie);

                for (int i = 0; item_category_string[i] != NULL; i++)
                {
                    if (strcmp(categorie, item_category_string[i]) == 0)
                    {
                        item->category = (ItemCategory)i;
                    }
                }
            }

            fscanf(file, "%s", title);
        }

        list_pushback(this, item);
    }

    return this;
}

void stocks_display(StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;
        printf("%04d %s %s\n", itemInStocks->id, itemInStocks->label, item_category_string[itemInStocks->category]);
    }
}

void stocks_display_consigned(StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;
        if (itemInStocks->isConsigned)
        {
             printf("%04d %s %s\n", itemInStocks->id, itemInStocks->label, item_category_string[itemInStocks->category]);
        }
    }
}

// Destruit la liste des stockes.
void stocks_destroy(StockList *this)
{
    list_destroy(this);
}

// Cherche un item dans les stocks
//
// Notes: return NULL si l'objet ne peux pas etre trouver
Item *stocks_lookup_item(StockList *stocks, BareCode barecode)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;

        if (itemInStocks->id == barecode)
        {
            return itemInStocks;
        }
    }

    return NULL;
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

#include <termios.h>

void setup_user_input(void)
{
    struct termios info;
    int result = 0;

    result = tcgetattr(0, &info);

    if (result == -1)
    {
        perror("Failled to setup user input");
        abort();
    }

    info.c_lflag &= ~(ECHO | ICANON);

    result = tcsetattr(0, TCSANOW, &info);
}

void restore_user_input(void)
{
    struct termios info;
    int result = 0;

    result = tcgetattr(0, &info);

    if (result == -1)
    {
        perror("Failled to setup user input");
        abort();
    }

    info.c_lflag |= (ECHO | ICANON);

    result = tcsetattr(0, TCSAFLUSH, &info);
}

typedef enum
{
    INPUT_INVALID,
    INPUT_VALID,
    INPUT_OK,
} InputValidState;

//void user_input_display(const char *format, const char *input)
//{
//    int format_index = 0;
//    int input_index = 0;
//}

InputValidState user_input_valid(const char *format, const char *input)
{
    for (int i = 0; input[i]; i++)
    {
        char in_c = input[i];
        if (format[i] == '_' && !(isalpha(in_c) || isdigit(in_c)))
            return INPUT_INVALID;

        if (format[i] == '.' && !isalpha(in_c))
            return INPUT_INVALID;

        if (format[i] == '#' && !isdigit(in_c))
            return INPUT_INVALID;
    }

    return INPUT_VALID;
}

typedef void (*ListCallback)(const char *user_input, void *args);

void user_input(const char *format, char *result, ListCallback list_callback, void *list_callback_args)
{
    (void)list_callback;
    (void)list_callback_args;
    char c;
    int index = 0;

    printf("\e[1;1H\e[2J");
    fflush(stdout);

    printf("\e[s");
    do
    {
        printf("\e[u");
        printf("\e[s");

        printf("\e[37m");

        printf("%s", format);

        printf("\e[0m");

        assert(read(STDIN_FILENO, &c, 1) == 1);

        printf("\e[u");

        printf("\e[s");

        if (c == 127 && index > 0)
        {
            index--;
            result[index] = '\0';
        }
        else if (iscntrl(c))
        {
            // do nothing with it
        }
        else if (index < (int)strlen(format))
        {
            result[index] = c;
            result[index + 1] = '\0';
            index++;
        }

        printf("%s%s\e[0m\n", user_input_valid(format, result) ? "\e[32m" : "\e[31m", result);

        printf("\e[J");

        if (list_callback)
        {
            list_callback(result, list_callback_args);
        }

        fflush(stdout);
    } while (c != '\n');
}

bool strStartWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);

    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

void autocomplete_client_list(const char *user_input, ClientsList *clients)
{
    list_foreach(item, clients)
    {
        Client *client = (Client *)item->value;

        char client_id_string[5];
        sprintf(client_id_string, "%03d", client->id);

        if (strStartWith(user_input, client_id_string))
        {
            printf("%04d %s %s\n", client->id, client->firstname, client->lastname);
        }
    }
}

void autocomplete_stock_list_consigned(const char *user_input, StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;

        char item_id_string[5];
        sprintf(item_id_string, "%03d", itemInStocks->id);

        if (strStartWith(user_input, item_id_string) && itemInStocks->isConsigned)
        {
            printf("%04d %s %5.2f€\n", itemInStocks->id, itemInStocks->label, itemInStocks->consignedValue);
        }
    }
}

void consigned_bottles_operation (StockList *stock)
{
    float totValue = 0.;
    int nb_bottles = 0;
    BareCode inserted_br;
    char choice='y', input[128];
    Item *found_item;

    while (choice == 'y')
    {
        printf("Inserez le codebarre de la bouteille a rendre : \n\n");
        stocks_display_consigned(stock);
        setup_user_input();
        user_input("####", input, (ListCallback)autocomplete_stock_list_consigned, (void *)stock);
        inserted_br = atoi(input);
        restore_user_input();

        //scanf("%4d", &inserted_br);
        found_item = stocks_lookup_item (stock, inserted_br);
        
        if (found_item != NULL && found_item->isConsigned)
        {
            printf("Entrez le nombre de bouteilles a rendre :\n> ");
            while (nb_bottles < 1)
            {
                scanf("%d", &nb_bottles);
                if (nb_bottles < 1) printf("Erreur, recommencez\n> ");
            }
            totValue += found_item->consignedValue * nb_bottles;
            nb_bottles = 0;
            printf("Vous allez recuperer %5.2f€, voulez-vous continuer ? (y/n) :\n> ", totValue);
            choice = ' ';
            while (!(choice == 'y' || choice == 'n'))
            {
                scanf(" %c", &choice);
                if (!(choice == 'y' || choice == 'n')) printf("Erreur, recommencez\n> ");
            }
        }
        else printf("Erreur, le codebarre entré ne correpond pas a un article consigne\n");   
    }
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    FILE *fStock, *fClient;
    fStock = fopen("stock.dat", "r");
    fClient = fopen("client.dat", "r");

    int choice = -1;

    // Lecture stock.dat
    StockList *stocks = stocks_create(fStock);

    /*printf("\nList des articles: \n");
    stocks_display(stocks);*/

    // Lecture client.dat
    ClientsList *clients = clients_create(fClient);

    /*printf("\nList des clients:\n");
    clients_display(clients);

    char input[128];
    setup_user_input();
    user_input("####", input, (ListCallback)autocomplete_client_list, (void *)clients);
    printf("\nuserinput: %s\n", input);
    restore_user_input();*/

    // menu
    printf("1 : Effectuer un achat\n2 : Rendre des bouteilles consignées \n3 : Sortir du programme\n\n");

    while (choice < 0 || choice > 3)
    {
        printf("> ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // Effectuer un achat
            printf("vous avez choisi d'effectuer un achat\n");
            break;
        
        case 2: // Rendrer des bouteilles consignées
            printf("vous avez choisi de rendre des bouteilles consignées\n");
            consigned_bottles_operation(stocks);

            break;

        case 3: // Fin du programme 
            break;

        default:
            printf("Erreur, recommencez\n");
        }
        
    }

    printf("Fin du programme ...\n");

    stocks_destroy(stocks);
    clients_destroy(clients);

    return 0;
}
