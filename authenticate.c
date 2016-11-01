//
// Created by piotr on 26.10.16.
//

#include "authenticate.h"

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <security/pam_modules.h>

const int AUTH_OK = 1;
const int AUTH_FAIL = 0;

static struct pam_conv login_conv = {
        misc_conv,
        NULL
};

int authenticate() {
    int retval = -1;
    pam_handle_t *pamh = NULL;

    retval = pam_start("sudo", NULL, &login_conv, &pamh);
    if (pamh == NULL || retval != PAM_SUCCESS)
        return AUTH_FAIL;

    retval = pam_set_item(pamh, PAM_USER_PROMPT, "Badania korpusowe?\n\r");

    retval = pam_authenticate(pamh, 0);
    pam_end(pamh, retval);
    if (retval != PAM_SUCCESS)
        return AUTH_FAIL;
    else
        return AUTH_OK;
}
