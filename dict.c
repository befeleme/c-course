#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dict.h"
#include "word.h"

const char SINGLE_KEY[] = "boryna";
const char DUMMY_KEY[] = "jagna";

struct dict {
    dict_value count;
};

struct dict_iterator {
    int position;
    dict_key current_key;
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

dict_iterator *dict_iterator_new(dict *d) {
    dict_iterator *result = malloc(sizeof(dict_iterator));
    if (!result) {
        return NULL;
    }
    result->position = 0;
    result->current_key = NULL;
    return result;
}

void dict_iterator_free(dict_iterator *it)
{
    if (it->current_key) {
        word_free(it->current_key);
    }
    free(it);
}

int dict_iterator_next(dict_iterator *it, dict_key *key) {
    if (it->current_key) {
        word_free(it->current_key);
        it->current_key = NULL;
    }
    int result = -1;
    const char *string;
    switch (it->position) {
        case 0: string = SINGLE_KEY; break;
        case 1: string = DUMMY_KEY; break;
        default:
            *key = NULL;
            return 0;
    }
    it->current_key = word_alloc();
    if (!it->current_key) {
        fprintf(stderr, "could not allocate key\n");
        goto finally;
    }

    for (string; *string; string++) {
        if (word_add_char(it->current_key, *string) < 0) {
            fprintf(stderr, "could not add letter to key\n");
            goto finally;
        }
    }
    it->position++;
    *key = it->current_key;
    result = 1;
    
finally:
    return result;
}