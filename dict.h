/* A data type and functions for a mapping */

typedef struct dict dict;

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