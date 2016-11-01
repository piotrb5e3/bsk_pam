//
// Created by piotr on 10/29/16.
//

#ifndef ZAD_PAM_WORD_READER_H
#define ZAD_PAM_WORD_READER_H

#include <stdlib.h>

const int WR_OK;
const int WR_FAIL;
const int WR_TXT_END;
const int WR_PROG_END;

int wr_get_word(char *out_word, size_t max_word_length);

int wr_cleanup();

const char *wr_get_first_line();

int wr_init();
#endif //ZAD_PAM_WORD_READER_H
