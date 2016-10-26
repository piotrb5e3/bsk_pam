//
// Created by piotr on 26.10.16.
//

#include "pam_auth.h"

#include <security/pam_appl.h>
#include <security/pam_misc.h>

const int AUTH_OK = 1;
const int AUTH_FAIL = 0;

static struct pam_conv login_conv = {
        misc_conv,
        NULL
};

int pam_auth() {
    pam_handle_t *pamh = NULL;
    int retval = -1;

    retval = pam_start("sudo", NULL, &login_conv, &pamh);
    if (pamh == NULL || retval != PAM_SUCCESS)
        return AUTH_FAIL;

    retval = pam_authenticate(pamh, 0);
    if (retval != PAM_SUCCESS)
        return AUTH_FAIL;
    pam_end(pamh, PAM_SUCCESS);
    return AUTH_OK;
}