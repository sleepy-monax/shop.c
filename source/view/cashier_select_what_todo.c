#include <stdlib.h>

#include "view/views.h"
#include "utils/input.h"
#include "utils/string.h"

#include "shop/basket.h"

void autocomplete_stock_list(const char *user_input, StockList *stocks)
{
    list_foreach(item, stocks)
    {
        Item *itemInStocks = (Item *)item->value;

        char item_id_string[5];
        sprintf(item_id_string, "%03d", itemInStocks->id);

        if (str_start_with(user_input, item_id_string))
        {
            printf("%04d %s %5.2f€ ", itemInStocks->id, itemInStocks->label, itemInStocks->price);
            if (itemInStocks->reduction != 0)
            {
                printf("%3d %% \n", itemInStocks->reduction);
            }
            else
                printf("\n");
        }
    }
}

void cashier_select_what_todo(StockList *stock)
{
    int quantity = -1;
    float totValue = 0.;
    BareCode inserted_br;
    char choice = 'y', input[128];
    Item *found_item;
    Basket *basket = basket_create();

    while (choice == 'y')
    {
        stocks_display(stock);
        user_input("Inserez le codebarre de l'article", "####", input);

        inserted_br = atoi(input);

        found_item = stocks_lookup_item(stock, inserted_br);

        if (found_item != NULL)
        {
            printf("Entrez la quatite que vous souhaitez acheter : \n> ");

            while (quantity < -1)
            {
                scanf("%d", &quantity);
                if (quantity < -1)
                    printf("Erreur, recommencez\n> ");
            }
            if (quantity == 0)
                printf("nous avez annule cet achat\n");

            if (quantity > 0)
            {
                basket_add_item(basket, found_item, quantity);

                totValue = found_item->price;
                if (found_item != NULL)
                    totValue -= found_item->price * found_item->reduction;
                totValue *= quantity;
            }
        }

        printf("Voulez-vous continuer a ajouter des articles au panier ? (y/n)\n> ");
        choice = ' ';
        while (!(choice == 'y' || choice == 'n'))
        {
            scanf(" %c", &choice);

            if (!(choice == 'y' || choice == 'n'))
                printf("Erreur, recommencez\n> ");
        }
    }

    printf("voici le contenu du panier : \n\n");
    basket_print_bill(basket);
}