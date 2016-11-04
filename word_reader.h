#ifndef ZAD_PAM_WORD_READER_H
#define ZAD_PAM_WORD_READER_H

#include <stdlib.h>

extern const int WR_OK;
extern const int WR_FAIL;
extern const int WR_TXT_END;
extern const int WR_PROG_END;

int wr_get_word(char *out_word, size_t max_word_length);

int wr_cleanup();

const char *wr_get_first_line();

int wr_init();
#endif //ZAD_PAM_WORD_READER_H
