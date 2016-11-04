#include <stdio.h>
#include "authenticate.h"
#include "counter.h"

#define STATUS_OK 0
#define STATUS_FAIL 1
#define STATUS_END 2

int handle_single_txt();


int main(int argc, char **argv) {
    if (authenticate() == AUTH_OK) {
        printf("Auth ok!\n");
        while (1) {
            int ret = handle_single_txt();
            if (ret == STATUS_FAIL) {
                return 1;
            } else if (ret == STATUS_END) {
                break;
            }
        }
        if (cleanup() != COUNTER_OK) {
            return 1;
        }
    } else {
        printf("Auth failed!\n");
    }
    return 0;
}

int handle_single_txt() {
    int ret;
    ret = count_words();
    if (ret == COUNTER_FAIL) {
        return STATUS_FAIL;
    } else if (ret == COUNTER_END) {
        return STATUS_END;
    }

    char *words[5];
    int counts[5];

    ret = get_5_top_words(words, counts);

    printf("Got %d words:\n\r", ret);
    for (int i = 0; i < ret; i++) {
        printf("Word: %s, count: %d\n\r", words[i], counts[i]);
    }
    return STATUS_OK;
}