#include <stdlib.h>
#include "shop/session.h"

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

void session_print_bill(Session *session)
{
    basket_print_bill(session->basket);
    if (session != NULL)
        printf("\n\nCher mr/mme %-s, vous avez a present %d points !\n", session->client->lastname, session->client->points);
}