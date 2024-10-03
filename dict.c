#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "word.h"

const char SINGLE_KEY[] = "boryna";

struct dict {
    dict_value count;
};

dict *dict_alloc(void)
{
    dict *result = malloc(sizeof(dict));
    if (!result) {
        return NULL;
    }
    result->count = 0;
    return result;
}

void dict_free(dict *d) {
    free(d);
}

int dict_set(dict *d, dict_key key, dict_value value) {
    int result = -1;
    char *key_data = word_get_data(key);
    if (!key_data) {
        goto finally;
    }

    if (strcmp(key_data, SINGLE_KEY) == 0) {
        d->count = value;
    }
    result = 0;
finally:
    word_free(key);
    return result;
}

int dict_get(dict *d, dict_key key, dict_value *value) {
    int result = -1;
    char *key_data = word_get_data(key);
    if (!key_data) {
        *value = -1;
        goto finally;
    }
    if (strcmp(key_data, SINGLE_KEY) == 0) {
        *value = d->count;
        result = 1;  // found
    } else {
        *value = -1;
        result = 0; // not found
    }
finally:
    return result;
}