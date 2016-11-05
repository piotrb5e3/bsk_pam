#include "authenticate.h"

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <security/pam_modules.h>

#include <stdbool.h>

const int AUTH_OK = 1;
const int AUTH_FAIL = 0;

static struct pam_conv login_conv = {
        misc_conv,
        NULL
};

bool pam_status_ok(pam_handle_t *pamh, int pam_status) {
    if (pam_status != PAM_SUCCESS) {
        fprintf(stderr, "%s\n\r", pam_strerror(pamh, pam_status));
        return false;
    }
    return true;
}

int authenticate() {
    int retval = -1;
    pam_handle_t *pamh = NULL;

    retval = pam_start("korpusowe", NULL, &login_conv, &pamh);
    if (pamh == NULL || !pam_status_ok(pamh, retval))
        return AUTH_FAIL;

    retval = pam_set_item(pamh, PAM_USER_PROMPT, "Badania korpusowe?\n\r");

    retval = pam_authenticate(pamh, 0);
    if (!pam_status_ok(pamh, retval)) {
        retval = pam_end(pamh, retval);
        pam_status_ok(pamh, retval);
        return AUTH_FAIL;
    } else {
        retval = pam_end(pamh, retval);
        if (!pam_status_ok(pamh, retval)) {
            return AUTH_FAIL;
        }
        return AUTH_OK;
    }
}
