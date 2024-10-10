#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "demo.h"

// run with: gcc -Wall --std=c99 -pedantic demo.c test.c  -Wimplicit-fallthrough  -o demo && ./demo


typedef struct llist_entry {
    llist_item_type item;
    struct llist_entry *prev;
} llist_entry;

struct llist_type {
    struct llist_entry *head;
    ssize_t count;
};


llist_type *llist_new(void)
{
    llist_type *list = malloc(sizeof(llist_type));
    list->head = NULL;
    list->count = 0;
    return list;
}

int llist_push(llist_type *list, llist_item_type item)
{
    llist_entry *new_entry = malloc(sizeof(llist_entry));
    if (new_entry == NULL) {
        return -1;
    }
    // new_entry is a pointer, so assigning must be with ->, not .
    new_entry->item = item;
    new_entry->prev = list->head;
    // list->head must be set to the latest object in the llist
    list->head = new_entry;
    list->count++;
    return 0;
}

int llist_pop(llist_type *list, llist_item_type *result)
{
    // end of the list, no more entries to pop
    if (list->head == NULL) {
        *result = 0;
        return -1;
    }
    *result = list->head->item;
    llist_entry *current_entry = list->head;
    list->head = list->head->prev;
    free(current_entry);
    list->count--;
    return 0;
}

void llist_free(llist_type *list)
{
    llist_item_type ignored_result;
    while (list->head) {
        int result = llist_pop(list, &ignored_result);
        assert(result == 0);
        list->count = 0;
    }
    free(list);
}

int llist_dump(llist_type *list)
{
    if (list->head == NULL) {
        return -1;
    }
    llist_entry *current = list->head;
    while (current) {
        printf("%d\n", current->item);
        current = current->prev;
    }
    return 0;
}

ssize_t llist_count(llist_type *list)
{
    return list->count;
}

int llist_get(llist_type *list, ssize_t n, llist_item_type *result) {
    for (llist_entry *current = list->head; current; current = current->prev) {
        if (n-- == 0) {
            *result = current->item;
            return 0;
        }
    }
    // we iterated till the end and haven't found the nth element
    *result = 0;
    return -1;
}

ssize_t llist_remove_first_n(llist_type *list, ssize_t n)
{
    llist_item_type ignored_result;

    for (ssize_t n_popped_items = 0; n_popped_items < n;n_popped_items++) {
        if (list->head == NULL) {
            return n_popped_items;
        }
        if (llist_pop(list, &ignored_result) == -1) {
            return -1;
        }
    }
    return n;
}

int llist_remove(llist_type *list, ssize_t n, llist_item_type *result)
{
// *ptr_to_update points to the current entry
// ptr_to_update is the pointer which we need to update if we remove that entry

    for (
        llist_entry **ptr_to_update = &list->head;
         *ptr_to_update != (llist_entry *)NULL;
         ptr_to_update = &((*ptr_to_update)->prev)
    ) {
        if (n-- == 0) {
            llist_entry *entry_to_delete = *ptr_to_update;
            *result = entry_to_delete->item;
            *ptr_to_update = entry_to_delete->prev;
            free(entry_to_delete);
            list->count--;
            return 0;
        }
    }
    *result = 0;
    return -1;
}
