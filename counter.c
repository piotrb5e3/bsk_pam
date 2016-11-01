//
// Created by piotr on 10/29/16.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "counter.h"
#include "word_reader.h"

const int WORD_COUNTS_INIT_LEN = 128;

const int COUNTER_OK = 0;
const int COUNTER_END = 4;
const int COUNTER_FAIL = 1;
const int COUNTER_FOUND = 2;
const int COUNTER_NOT_FOUND = 3;


typedef struct {
    int count;
    char word[WORDLEN];
} wc;

typedef wc *t_word_count;

t_word_count *word_counts = NULL;
size_t word_counts_size = 0;
size_t max_word_counts_size = 0;


int cleanup() {
    if (max_word_counts_size == 0) {
        fprintf(stderr, "Cleanup called on not initialised word_counts\n\r");
        return COUNTER_FAIL;
    } else {
        for (int i = 0; i < word_counts_size; i++) {
            free(word_counts[i]);
        }
        free(word_counts);
        word_counts = NULL;
    }
    if(wr_cleanup() != WR_OK) {
        return COUNTER_FAIL;
    } else {
        return COUNTER_OK;
    }
}


int init_word_counts() {
    if (word_counts != NULL)
        cleanup();
    word_counts = calloc(WORD_COUNTS_INIT_LEN, sizeof(t_word_count));
    if (word_counts == NULL) {
        fprintf(stderr, "Failed to allocate initial word_counts!\n\r");
        return COUNTER_FAIL;
    }
    max_word_counts_size = WORD_COUNTS_INIT_LEN;
    word_counts_size = 0;
    return COUNTER_OK;
}


int enlarge() {
    size_t new_size = max_word_counts_size * 2;
    word_counts = realloc(word_counts, new_size * sizeof(*word_counts));
    if (word_counts == NULL) {
        fprintf(stderr, "Array enlarge failed.");
        return COUNTER_FAIL;
    }
    max_word_counts_size = new_size;
    return COUNTER_OK;
}


int enlarge_if_needed() {
    if (word_counts_size >= max_word_counts_size) {
        if (enlarge() == COUNTER_FAIL)
            return COUNTER_FAIL;
    }

    return COUNTER_OK;
}


int increase_counter_if_word_exists(const char *word) {
    char w[WORDLEN];
    strncpy(w, word, WORDLEN);
    for (int i = 0; i < word_counts_size; i++) {
        const char *candidate = (char *) word_counts[i]->word;
        if (strncmp(candidate, word, WORDLEN) == 0) {
            word_counts[i]->count++;
            return COUNTER_FOUND;
        }
    }
    return COUNTER_NOT_FOUND;
}


int add_new_word(const char *word) {
    if (enlarge_if_needed() == COUNTER_FAIL)
        return COUNTER_FAIL;

    t_word_count new_word = calloc(1, sizeof *new_word);
    if (new_word == NULL) {
        fprintf(stderr, "Failed to allocate memory for new_word!\n\r");
        return COUNTER_FAIL;
    }
    new_word->count = 1;
    strncpy(new_word->word, word, WORDLEN);
    word_counts[word_counts_size] = new_word;
    word_counts_size++;
    return COUNTER_OK;
}


int count_word(const char *word) {
    if (increase_counter_if_word_exists(word) == COUNTER_FOUND) {
        return COUNTER_OK;
    } else {
        return add_new_word(word);
    }
}


int count_words() {
    char tmp_word[WORDLEN];
    const char *first_line = NULL;
    int ret = 0;

    ret = init_word_counts();
    if (ret == COUNTER_FAIL)
        return COUNTER_FAIL;

    ret = wr_init();
    if (ret != WR_OK)
        return COUNTER_FAIL;

    while (1) {
        ret = wr_get_word(tmp_word, WORDLEN);
        if (ret == WR_FAIL) {
            return COUNTER_FAIL;
        } else if (ret == WR_PROG_END) {
            return COUNTER_END;
        } else if (ret == WR_TXT_END) {
            break;
        } else {
            ret = count_word(tmp_word);
            if (ret == COUNTER_FAIL) {
                return COUNTER_FAIL;
            }
        }
    }

    first_line = wr_get_first_line();
    if(first_line == NULL) {
        fprintf(stderr, "First line is null\n\r");
        return COUNTER_FAIL;
    }
    printf("First line:\n\r%s\n\r", wr_get_first_line());

    return COUNTER_OK;
}


void insert_at_and_keep_size(t_word_count *arr, size_t arr_len, size_t n, t_word_count what) {
    for (size_t i = arr_len - 1; i > n; i--) {
        arr[i] = arr[i - 1];
    }
    arr[n] = what;
}


/// Returns count of words returned
size_t get_5_top_words(char **out_words, int *out_counts) {
    size_t words_to_return = (word_counts_size < 5) ? word_counts_size : 5;

    if (words_to_return <= 0) {
        return 0;
    }
    t_word_count *candidates = calloc(words_to_return, sizeof(*candidates));
    if (candidates == NULL) {
        fprintf(stderr, "Failed to allocate memory for candidates!\n\r");
        return 0;
    }

    size_t word_ptr = 0;

    while (word_ptr < words_to_return) {
        size_t new_pos = 0;
        while (new_pos < word_ptr
               && candidates[new_pos]->count > word_counts[word_ptr]->count) {
            new_pos++;
        }
        insert_at_and_keep_size(candidates, words_to_return, new_pos, word_counts[word_ptr]);
        word_ptr++;
    }

    while (word_ptr < word_counts_size) {
        size_t new_pos = 0;
        while (new_pos < 5
               && candidates[new_pos]->count > word_counts[word_ptr]->count) {
            new_pos++;
        }
        if (new_pos < 5)
            insert_at_and_keep_size(candidates, words_to_return, new_pos, word_counts[word_ptr]);
        word_ptr++;
    }

    for (int i = 0; i < words_to_return; i++) {
        out_words[i] = candidates[i]->word;
        out_counts[i] = candidates[i]->count;
    }
    free(candidates);
    return words_to_return;

}
