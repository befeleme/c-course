/* A data type and functions for a mapping */

typedef struct dict dict;
typedef struct dict_iterator dict_iterator;

// Parametrization of the dict
typedef struct word * dict_key;
typedef int dict_value;

// dict API
dict *dict_alloc(void);
void dict_free(dict *d);

// 0 = success, -1 = fail
// after the call, key is owned by the dict; the caller should no longer use it
int dict_set(dict *d, dict_key key, dict_value value);
// 1 = found, 0 = not found, -1 = fail
int dict_get(dict *d, dict_key key, dict_value *result);

// NULL on failure
dict_iterator *dict_iterator_new(dict *d);

void dict_iterator_free(dict_iterator *it);
// 1 = found, 0 = at end (output set to NULL/-1), -1 = fail
// the dict owns the resulting key; it is valid until the next dict operation (except dict_get)
int dict_iterator_next(dict_iterator *it, dict_key *key);