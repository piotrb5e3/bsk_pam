#include "word_reader.h"

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>


int eq_count = 0;

const int WR_OK = 0;
const int WR_FAIL = 1;
const int WR_TXT_END = 2;
const int WR_PROG_END = 3;

const int FIRST_LINE_INIT_LEN = 128;

char *first_line = NULL;
size_t fline_ptr = 0;
size_t fline_max_size = 0;

bool is_first_line = true;
bool last_was_newline = true;

int fline_enlarge() {
    size_t new_size = fline_max_size * 2;
    first_line = realloc(first_line, new_size * sizeof(*first_line));
    if (first_line == NULL) {
        fprintf(stderr, "first_line array enlarge failed.");
        return WR_FAIL;
    }
    fline_max_size = new_size;
    return WR_OK;
}

int fline_add_char(char c) {
    int ret;
    if (fline_ptr >= fline_max_size - 1) {
        ret = fline_enlarge();
        if (ret != WR_OK)
            return ret;
    }
    first_line[fline_ptr] = c;
    first_line[fline_ptr + 1] = '\0';
    fline_ptr++;
    return WR_OK;
}

int init_first_line() {
    first_line = calloc(FIRST_LINE_INIT_LEN, sizeof(*first_line));
    if (first_line == NULL) {
        fprintf(stderr, "Failed to allocate memory for first txt line storage!\n\r");
        return WR_FAIL;
    }
    fline_max_size = FIRST_LINE_INIT_LEN;
    fline_ptr = 0;
    return WR_OK;
}

int wr_init() {
    if (first_line != NULL)
        wr_cleanup();
    is_first_line = true;
    return init_first_line();
}

int wr_cleanup() {
    if (fline_max_size == 0) {
        fprintf(stderr, "Cleanup called on not initialised first_line\n\r");
        return WR_FAIL;
    } else {
        free(first_line);
        first_line = NULL;
        return WR_OK;
    }
}


int wr_get_word(char *out_word, size_t word_arr_length) {
    size_t wptr = 0;
    int c;
    do {
        c = fgetc(stdin);

        if (c == '\n' || c == '\r') {
            is_first_line = false;
            last_was_newline = true;
        }

        if ((c == '\n' || c == '\r' || c == EOF) && eq_count == 2) {
            eq_count = 0;
            return WR_TXT_END;
        }

        if (is_first_line) {
            if(fline_add_char(c) != WR_OK)
                return WR_FAIL;
        }

        if (c == '=' && (last_was_newline || eq_count == 1)) {
            eq_count++;
        } else {
            eq_count = 0;
        }

        if (c == '.') {
            return WR_PROG_END;
        }

        if (c == EOF) {
            fprintf(stderr, "Unexpected file end. Expected >>.<<!\n\r");
            return WR_FAIL;
        } else if (isalpha(c)) {
            out_word[wptr] = c;
            wptr++;
        } else if (wptr > 0) {
            break;
        }

        if (c != '\n' && c != '\r' && last_was_newline) {
            last_was_newline = false;
        }
    } while (wptr < word_arr_length - 1);
    for (size_t i = wptr; i < word_arr_length; i++) {
        out_word[i] = '\0';
    }
    return WR_OK;
}

const char *wr_get_first_line() {
    return first_line;
}