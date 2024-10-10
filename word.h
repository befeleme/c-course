#include <stdlib.h>

typedef struct word word;

word *word_alloc(void);

void word_free(word *w);

int word_add_char(word *w, char c);

char *word_get_data(word *w);

size_t word_size(word *w);