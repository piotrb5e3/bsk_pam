#ifndef ZAD_PAM_COUNTER_H
#define ZAD_PAM_COUNTER_H

#define WORD_ARR_LEN 1024

const int COUNTER_OK;
const int COUNTER_END;
const int COUNTER_FAIL;

int count_words();

size_t get_5_top_words(char **out_words, int *out_counts);

int cleanup();

#endif //ZAD_PAM_COUNTER_H
