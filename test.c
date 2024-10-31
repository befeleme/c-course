#include "word.h"
#include "demo.h"
#include <assert.h>
#include <string.h>

int main() {

    char *boryna = "boryna";
    char *jagna = "jagna";

    word *a = word_from_string(boryna);
    word *b = word_from_string(boryna);
    word *c = word_from_string(jagna);

    assert(word_equal(a, b) == 1);
    assert(word_equal(a, c) == 0);
    assert(word_equal(b, c) == 0);
    assert(strcmp(word_get_data(a), boryna) == 0);
    assert(strcmp(word_get_data(c), jagna) == 0);
    assert(strcmp(word_get_data(b), jagna) == 1);

    word_free(a);
    word_free(b);
    word_free(c);
}
