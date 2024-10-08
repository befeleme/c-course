#include <sys/types.h>

typedef int llist_item_type;

typedef struct llist_type llist_type;

llist_type *llist_new(void); // returns NULL on error
void llist_free(llist_type *list); // llist_new calls malloc, we need to free the list at the end

int llist_push(llist_type *list, llist_item_type item);
// returns -1 on error, 0 on success
int llist_pop(llist_type *list, llist_item_type *result);
// on error, returns -1, sets *result to zero
// on success, returns 0 and sets *result

int llist_dump(llist_type *list);
// print list contents using print, one number per line, either order
// on error, returns -1
// on success, returns 0

ssize_t llist_count(llist_type *list);
// return number of elements
// on error, returns -1

int llist_get(llist_type *list, ssize_t n, llist_item_type *result);
// return n'th element
// on error, returns -1, sets *result to zero
// on success, returns 0 and sets *result

int llist_remove(llist_type *list, ssize_t n, llist_item_type *result);
// remove n'th element
// on error, returns -1, sets *result to zero
// on success, returns 0 and sets *result to removed item

ssize_t llist_remove_first_n(llist_type *list, ssize_t n);
// remove up to n elements from the start of the list
// (this may be less than n if the list is short)
// on error, returns -1
// on success, returns the number of removed elements

// d.u.
int llist_dump(llist_type *list);
// print list contents in both directions
// on error, returns -1
// on success, returns 0

void llist_check(llist_type *list);
// asserts that each entry of the list is correct:
//  if there's a next item, its prev points to the current one
//  if there's a prev item, its next points to the current one
//  assert that count is correct

#ifdef NDEBUG
// in "release mode", llist_check does nothing
// "assert" is skipped there
#define llist_check(list)
#endif