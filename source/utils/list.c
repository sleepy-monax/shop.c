#include <stdlib.h>
#include "list.h"

List *list_create(void)
{
    List *this = malloc(sizeof(List));

    this->count = 0;
    this->head = NULL;
    this->tail = NULL;

    return this;
}

void list_destroy(List *this)
{
    list_clear(this);
    free(this);
}

List *list_clone(List *this)
{
    List *copy = list_create();

    list_foreach(i, this)
    {
        list_pushback(copy, i->value);
    }

    return copy;
}

void list_clear(List *this)
{
    ListItem *current = this->head;

    while (current)
    {
        ListItem *next = current->next;

        free(current->value);
        free(current);
        current = next;
    }

    this->count = 0;
    this->head = NULL;
    this->tail = NULL;
}

void list_insert_sorted(List *this, void *value, ListComparator comparator)
{
    if (this->head == NULL || comparator(value, this->head->value))
    {
        list_push(this, value);
    }
    else
    {
        ListItem *current = this->head;

        while (current->next != NULL && comparator(current->next->value, value))
        {
            current = current->next;
        }

        ListItem *item = malloc(sizeof(ListItem));

        item->prev = current;
        item->next = current->next;
        item->value = value;

        if (current->next == NULL)
        {
            this->tail = item;
        }
        else
        {
            current->next->prev = item;
        }

        current->next = item;

        this->count++;
    }
}

bool list_peek(List *this, void **value)
{
    if (this->head != NULL)
    {
        *value = this->head->value;
        return true;
    }
    else
    {
        *value = NULL;
        return false;
    }
}

bool list_peekback(List *this, void **value)
{
    if (this->tail != NULL)
    {
        *value = this->tail->value;

        return true;
    }
    else
    {
        return false;
    }
}

static void list_peekat_from_head(List *this, int index, void **value)
{
    ListItem *current = this->head;

    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    *value = current->value;
}

static void list_peekat_from_back(List *this, int index, void **value)
{
    ListItem *current = this->tail;

    for (int i = 0; i < (this->count - index - 1); i++)
    {
        current = current->prev;
    }

    *value = current->value;
}

bool list_peekat(List *this, int index, void **value)
{
    if (this->count >= 1 && index >= 0 && index < this->count)
    {
        if (index < this->count / 2)
        {
            list_peekat_from_head(this, index, value);
        }
        else
        {
            list_peekat_from_back(this, index, value);
        }

        return true;
    }
    else
    {
        return false;
    }
}

void list_push(List *this, void *value)
{
    ListItem *item = malloc(sizeof(ListItem));

    item->prev = NULL;
    item->next = NULL;
    item->value = value;

    this->count++;

    if (this->head == NULL)
    {
        this->head = item;
        this->tail = item;
    }
    else
    {
        this->head->prev = item;
        item->next = this->head;
        this->head = item;
    }
}

bool list_pop(List *this, void **value)
{
    ListItem *item = this->head;

    if (this->count == 0)
    {
        return false;
    }
    else if (this->count == 1)
    {
        this->count = 0;
        this->head = NULL;
        this->tail = NULL;
    }
    else if (this->count > 1)
    {
        item->next->prev = NULL;
        this->head = item->next;

        this->count--;
    }

    if (value != NULL)
        *(value) = item->value;

    free(item);

    return true;
}

void list_pushback(List *this, void *value)
{
    ListItem *item = malloc(sizeof(ListItem));

    item->prev = NULL;
    item->next = NULL;
    item->value = value;

    this->count++;

    if (this->tail == NULL)
    {
        this->tail = item;
        this->head = item;
    }
    else
    {
        this->tail->next = item;
        item->prev = this->tail;
        this->tail = item;
    }
}

bool list_popback(List *this, void **value)
{
    ListItem *item = this->tail;

    if (this->count == 0)
    {
        return NULL;
    }
    else if (this->count == 1)
    {
        this->count = 0;
        this->head = NULL;
        this->tail = NULL;
    }
    else if (this->count > 1)
    {
        item->prev->next = NULL;
        this->tail = item->prev;

        this->count--;
    }

    if (value != NULL)
        *(value) = item->value;

    free(item);

    return true;
}

bool list_remove(List *this, void *value)
{
    list_foreach(item, this)
    {
        if (item->value == value)
        {
            if (item->prev != NULL)
            {
                item->prev->next = item->next;
            }
            else
            {
                this->head = item->next;
            }

            if (item->next != NULL)
            {
                item->next->prev = item->prev;
            }
            else
            {
                this->tail = item->prev;
            }

            this->count--;
            free(item->value);
            free(item);

            return true;
        }
    }

    return false;
}

bool list_containe(List *this, void *value)
{
    list_foreach(item, this)
    {
        if (item->value == value)
        {
            return true;
        }
    }

    return false;
}