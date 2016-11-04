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

    retval = pam_start("korpusowe", NULL, &login_conv, &pamh);
    if (pamh == NULL || retval != PAM_SUCCESS)
        return AUTH_FAIL;

    retval = pam_set_item(pamh, PAM_USER_PROMPT, "Badania korpusowe?\n\r");

    retval = pam_authenticate(pamh, 0);
    if (retval != PAM_SUCCESS) {
        retval = pam_end(pamh, retval);
        if (retval != PAM_SUCCESS)
            fprintf(stderr, "Pam_end failed!\n\r");
        return AUTH_FAIL;
    } else {
        retval = pam_end(pamh, retval);
        if (retval != PAM_SUCCESS) {
            fprintf(stderr, "Pam_end failed!\n\r");
            return AUTH_FAIL;
        }
        return AUTH_OK;
    }
}
