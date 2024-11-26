#include "common.h"

bool check_username(const char *username) {
    if (strlen(username) <= 5) {
        return false;
    }

    for (size_t i = 0; username[i] != '\0'; i++) {
        if (!(isalnum(username[i]) || username[i] == '_')) {
            return false;
        }
    }

    return true;
}

bool check_password(const char *password) {
    if (strlen(password) < 8) {
        return false;
    }

    bool has_upper = false, has_lower = false, has_digit = false, has_special = false;
    for (size_t i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) {
            has_upper = true;
        } else if (islower(password[i])) {
            has_lower = true;
        } else if (isdigit(password[i])) {
            has_digit = true;
        } else if (ispunct(password[i])) {
            has_special = true;
        }
    }

    return has_upper && has_lower && has_digit && has_special;
}
