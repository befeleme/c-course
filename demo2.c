#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "demo.h"

// run with: gcc -Wall --std=c99 -pedantic demo2.c test.c  -Wimplicit-fallthrough  -o demo && ./demo


typedef struct llist_entry {
    llist_item_type item;
    struct llist_entry *prev;
    struct llist_entry *next;
} llist_entry;

struct llist_type {
    ssize_t count;
    llist_entry base;
};


llist_type *llist_new(void)
{
    llist_type *list = malloc(sizeof(llist_type));
    if (list == NULL) {
        return NULL;
    }
    list->base.item = -1;
    list->base.prev = &(list->base);
    list->base.next = &(list->base);
    list->count = 0;
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
    new_entry->prev = list->base.prev;
    new_entry->next = &(list->base);

    list->base.prev->next = new_entry;
    list->base.prev = new_entry;

    list->count++;
    llist_check(list);
    return 0;
}

int llist_pop(llist_type *list, llist_item_type *result)
{
    llist_entry *last_entry = list->base.prev;

    if (last_entry == &(list->base)) {
        // nothing to pop
        *result = 0;
        return -1;
    }

    *result = last_entry->item;
    list->base.prev = last_entry->prev;
    list->base.prev->next = &(list->base);
    free(last_entry);
    list->count--;
    llist_check(list);
    return 0;
}

void llist_free(llist_type *list)
{
    llist_item_type ignored_result;
    while (list->base.prev != &(list->base)) {
        int result = llist_pop(list, &ignored_result);
        assert(result == 0);
    }
    free(list);
}

ssize_t llist_count(llist_type *list)
{
    return list->count;
}

int llist_get(llist_type *list, ssize_t n, llist_item_type *result) {
    for (llist_entry *current = list->base.prev; current != &(list->base); current = current->prev) {
        if (n-- == 0) {
            *result = current->item;
            return 0;
        }
    }
    // we iterated till the end and haven't found the nth element
    *result = 0;
    return -1;
}

void entry_check(llist_entry *entry)
{
    assert(entry->next->prev = entry);
    assert(entry->prev->next = entry);
}


void llist_check(llist_type *list) {
    ssize_t count = 0;
    llist_entry *base = &(list->base);
    
    entry_check(base);
    for (llist_entry *current = base; current->next != base; current = current->next) {
        count++;
        entry_check(current);
    }
    assert(list->count == count);
    for (llist_entry *current = base; current->prev != base; current = current->prev) {
        count--;
        entry_check(current);
    }
    assert(count == 0);
}

int llist_dump(llist_type *list)
{

    for (llist_entry *current = list->base.prev; current != &(list->base); current = current->prev) {
        printf("%d\n", current->item);
    }
    return 0;
}

ssize_t llist_remove_first_n(llist_type *list, ssize_t n)
{
    llist_item_type ignored_result;

    for (ssize_t n_popped_items = 0; n_popped_items < n;n_popped_items++) {
        if (list->head == NULL) {
            llist_check(list);
            return n_popped_items;
        }
        if (llist_pop(list, &ignored_result) == -1) {
            llist_check(list);
            return -1;
        }
    }
    llist_check(list);
    return n;
}


int llist_remove(llist_type *list, ssize_t n, llist_item_type *result)
{
    for (llist_entry *current = base; current->prev != base; current = current->next) {
        if (n-- == 0) {
            *result = current->item;
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            list->count--;
            llist_check(list);
            return 0;
    }

    *result = 0;
    return -1;
}