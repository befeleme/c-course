#include <stdio.h>

#include "word.h"
#include "dict.h"

const char INPUT_FILE_NAME[] = "chlopi-processed.txt";
const size_t BUF_SIZE = 1024;

int main(void) {
    int result = 1;
    FILE* soubor = NULL;
    word* current_word = NULL;
    word* wanted_key = NULL;
    dict_iterator *iterator = NULL;
    char buffer[BUF_SIZE + 1];

    dict *words = dict_alloc();
    if (!words) {
        fprintf(stderr, "could not allocate dict\n");
        goto finally;
    }

    soubor = fopen(INPUT_FILE_NAME, "r");
    if (!soubor) {
        fprintf(stderr, "could not open file %s\n", INPUT_FILE_NAME);
        goto finally;
    }

    size_t read_count;
    current_word = word_alloc();
    if (!current_word) {
        fprintf(stderr, "could not allocate memory\n");
        goto finally;
    }
    while ((read_count = fread(buffer, 1, BUF_SIZE, soubor))) {
        printf("Nacetl jsem %zd bajtu\n", read_count);
        buffer[read_count] = 0;
        for (size_t i = 0; i < read_count; i++) {
            char c = buffer[i];
            if ((c == ' ') || (c == '\n')) {
                if (!word_size(current_word)) {
                    char *data = word_get_data(current_word);
                    if (!data) {
                        fprintf(stderr, "could not get word data\n");
                        goto finally;
                    }
                    printf("slovo: %s\n", data);

                    int count;
                    int result = dict_get(words, current_word, &count);
                    if (result < 0) {
                        fprintf(stderr, "could not get value from dict\n");
                        goto finally;
                    } else if (result == 0) {
                        count = 0;
                    }

                    result = dict_set(words, current_word, count +1);
                    current_word = NULL;
                    if (result < 0) {
                        fprintf(stderr, "could not set value in dict\n");
                        goto finally;
                    }

                    current_word = word_alloc();
                    if (!current_word) {
                        fprintf(stderr, "could not allocate memory\n");
                        goto finally;
                    }
            }
            } else {
                if (word_add_char(current_word, c) < 0) {
                    fprintf(stderr, "could not add character to word\n");
                    goto finally;
                }
            }
        }
    }

    if (!word_size(current_word)) {
        char *data = word_get_data(current_word);
        if (!data) {
            fprintf(stderr, "could not get word data\n");
            goto finally;
        }
        printf("slovo: %s\n", data);
    }

    word_free(current_word);
    current_word = NULL;

    int count;
    wanted_key = word_alloc();
    if (!wanted_key) {
        fprintf(stderr, "could not allocate wanted_key\n");
        goto finally;
    }

    for (char *ptr="boryna"; *ptr; ptr++) {
        if (word_add_char(wanted_key, *ptr) < 0) {
            fprintf(stderr, "could not add letter to wanted key\n");
            goto finally;
        }
    }
    if (dict_get(words, wanted_key, &count) < 0) {
        fprintf(stderr, "could not get value from dict\n");
        goto finally;
    }
    printf("value: %d\n", count);

    iterator = dict_iterator_new(words);
    if (!iterator) {
        fprintf(stderr, "could not iterate dict\n");
        goto finally;
    }

    int iteration_result;
    word *the_word;
    while ((iteration_result = dict_iterator_next(iterator, &the_word)) == 1) {
        int value;
        if (dict_get(words, the_word, &value) < 0) {
            fprintf(stderr, "could not get value from dict\n");
            goto finally;
        }
        char *data = word_get_data(the_word);
        if (!data) {
            fprintf(stderr, "could not get word data\n");
            goto finally;
        }
        printf("%s: %d\n", data, value);
    }
    if (iteration_result < 1) {
        fprintf(stderr, "could not iterate dict\n");
        goto finally;
    }

    result = 0;
finally:
    if (soubor) {
        fclose(soubor);
    }
    if (current_word) {
        word_free(current_word);
    }
    if (wanted_key) {
        word_free(wanted_key);
    }
    if (words) {
        dict_free(words);
    }
    if (iterator) {
        dict_iterator_free(iterator);
    }
    return result;
}
