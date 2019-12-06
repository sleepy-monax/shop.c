#pragma once

#include <stdbool.h>

#define list_foreach(item, list) for (ListItem *item = list->head; item != NULL; item = item->next)

typedef struct ListItem
{
    void *value;

    struct ListItem *prev;
    struct ListItem *next;
} ListItem;

typedef struct list
{
    int count;

    ListItem *head;
    ListItem *tail;
} List;

typedef bool (*ListComparator)(void *left, void *right);

List *list_create(void);

void list_destroy(List *this);

List *list_clone(List *this);

void list_clear(List *this);

void list_insert_sorted(List *this, void *value, ListComparator comparator);

bool list_peek(List *this, void **value);

bool list_peekback(List *this, void **value);

bool list_peekat(List *this, int index, void **value);

void list_push(List *this, void *value);

void list_pushback(List *this, void *value);

bool list_pop(List *this, void **value);

bool list_popback(List *this, void **value);

bool list_containe(List *this, void *value);

bool list_remove(List *this, void *value);

#define list_empty(__list) ((__list)->count == 0)

#define list_any(__list) ((__list)->count != 0)

#define list_count(__list) ((__list)->count)
