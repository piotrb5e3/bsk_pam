//
// Created by piotr on 26.10.16.
//
#include <stdio.h>
#include "pam_auth.h"

int main(int argc, char** argv) {
    if (pam_auth() == AUTH_OK) {
        printf("Auth ok!\n");
    } else {
        printf("Auth failed!\n");
        return 1;
    }
    return 0;
}