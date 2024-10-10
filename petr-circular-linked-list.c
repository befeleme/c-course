#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "demo.h"

// run with: gcc -Wall --std=c99 -pedantic demo.c test.c  -Wimplicit-fallthrough  -o demo && ./demo

typedef struct llist_entry {
    llist_item_type item;
    struct llist_entry *prev;
    struct llist_entry *next;
} llist_entry;

struct llist_type {
    ssize_t count;
    llist_entry base;
};

llist_entry *base(llist_type *list)
{
    return &(list->base);
}

llist_type *llist_new(void)
{
    llist_type *list = malloc(sizeof(llist_type));
    if (list == NULL) {
        return NULL;
    }
    list->count = 0;

    base(list)->item = -1;
    base(list)->prev = base(list);
    base(list)->next = base(list);
    llist_check(list);
    return list;
}

int llist_push(llist_type *list, llist_item_type item)
{
    llist_entry *new_entry = malloc(sizeof(llist_entry));
    if (new_entry == NULL) {
        return -1;
    }
    new_entry->item = item;
    new_entry->prev = base(list)->prev;
    new_entry->next = base(list);
    base(list)->prev->next = new_entry;
    base(list)->prev = new_entry;
    list->count++;
    llist_check(list);
    return 0;
}

int llist_pop(llist_type *list, llist_item_type *result)
{
    llist_entry *last_entry = base(list)->prev;
    if (last_entry == base(list)) {
        *result = 0;
        return -1;
    }
    *result = last_entry->item;
    base(list)->prev = last_entry->prev;
    base(list)->prev->next = base(list);
    free(last_entry);
    list->count--;
    llist_check(list);
    return 0;
}

void llist_free(llist_type *list)
{
    llist_item_type ignored_result;
    while (llist_pop(list, &ignored_result) == 0) {
        // nothing to do
    }
    free(list);
}

ssize_t llist_count(llist_type *list)
{
    return list->count;
}

int llist_get(llist_type *list, ssize_t n, llist_item_type *result)
{
    for (llist_entry *current = base(list)->prev;
         current != base(list);
         current = current->prev)
    {
        if (n-- == 0) {
            *result = current->item;
            return 0;
        }
    }
    *result = 0;
    return -1;
}

void entry_check(llist_entry *entry)
{
    assert(entry->next->prev == entry);
    assert(entry->prev->next == entry);
}

int llist_dump(llist_type *list)
{
    for (llist_entry *current = base(list)->next;
         current != base(list);
         current = current->next)
    {
        printf("%d\n", current->item);
    }
    return 0;
}

ssize_t llist_remove_first_n(llist_type *list, ssize_t n)
{
    llist_item_type ignored_result;
    ssize_t count = 0;
    while (count < n) {
        if (base(list)->prev == base(list)) {
            break;
        }
        if (llist_pop(list, &ignored_result) < 0) {
            return -1;
        }
        count++;
    }
    llist_check(list);
    return count;
}

int llist_remove(llist_type *list, ssize_t n, llist_item_type *result)
{
    for (llist_entry *current = base(list)->prev;
         current != base(list);
         current = current->prev)
    {
        if (n-- == 0) {
            *result = current->item;
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            list->count--;
            llist_check(list);
            return 0;
        }
    }
    *result = 0;
    return -1;
}

void llist_check(llist_type *list)
{
    ssize_t count = 0;
    entry_check(base(list));
    for (llist_entry *current = base(list)->next;
         current != base(list);
         current = current->next)
    {
        count++;
        entry_check(current);
    }
    assert(count == list->count);
    for (llist_entry *current = base(list)->prev;
         current != base(list);
         current = current->prev)
    {
        count--;
        entry_check(current);
    }
    assert(count == 0);
}