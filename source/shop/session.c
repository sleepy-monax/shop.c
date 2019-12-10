#include <stdlib.h>
#include "shop/session.h"
#include "utils/input.h"

Session *session_create(void)
{
    Session *session = malloc(sizeof(Session));

    session->basket = basket_create();
    session->client = NULL;

    return session;
}

void session_destroy(Session *session)
{
    basket_destroy(session->basket);
    free(session);
}

float calculate_total(Session *session)
{
    float total = 0., totalItem;

    list_foreach(item, session->basket)
    {
        BasketItem *b = (BasketItem *)item->value;

        totalItem = b->item->price;
        if (b->item->discount != 0)
            totalItem -= (b->item->discount / 100.0) * totalItem;
        totalItem *= b->quantity;

        total += totalItem;
    }
    return total;
}

void session_print_bill(Session *session)
{
    float totPrice_item, totPrice_item_discount = 0., totPriceBasket = 0., reduction = 0.;
    int lessPoints;

    totPriceBasket = calculate_total(session);

    if (session->client != NULL && session->client->points >= 500 && totPriceBasket >= 5)
        if (user_yes_no("Vous avez assez de points pour bénéficier de réductions, voulez vous les utiliser ?") == YES)
        {
            lessPoints = (int)totPriceBasket * 10;
            if (lessPoints > session->client->points)
                lessPoints = session->client->points;
            reduction = (float)lessPoints / 100;
            session->client->points -= lessPoints;
        }

    printf("voici le contenu du panier : \n\n");
    printf("Label                     Prix            Prix discount\n");
    printf("-------------------------------------------------------\n");

    list_foreach(item, session->basket)
    {
        BasketItem *b = (BasketItem *)item->value;

        totPrice_item = b->item->price;
        if (b->item->discount != 0)
        {
            totPrice_item_discount = totPrice_item - ((b->item->discount / 100.0) * totPrice_item);
            totPrice_item_discount *= b->quantity;
        }

        totPrice_item *= b->quantity;

        if (b->quantity == 1)
            printf("%-s                      %5.2f€", b->item->label, b->item->price);
        else
            printf("%-s  x %d      %5.2f€    %5.2f€", b->item->label, b->quantity, b->item->price, totPrice_item);

        if (totPrice_item_discount > 0.)
            printf("   %5.2f€\n", totPrice_item_discount);
        else
            printf("\n");
        totPrice_item_discount = 0.;
    }
    if (reduction == 0.)
        printf("\nTOTAL : %5.2f€\n", totPriceBasket);
    else
    {
        float newtot = totPriceBasket - reduction;
        printf("\nReduction : %5.2f€\nTOTAL : %5.2f€\n", reduction, newtot);
    }
}