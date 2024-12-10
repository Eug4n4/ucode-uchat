#include "server.h"

void print_log_priority(FILE *f, int priority) {
    switch (priority) {
    case LOG_INFO:
        fprintf(f, "[INFO] ");
        break;
    case LOG_ERR:
        fprintf(f, "[ERROR] ");
        break;
    default:
        break;
    }
}

void print_log_str_format(FILE *f, const char *format, va_list args) {
    while (*format != '\0') {
        if (*format == '%') {
            ++format;
            switch (*format) {
            case 'd':
                fprintf(f, "%d", va_arg(args, int));
                break;
            case 's':
                fprintf(f, "%s", va_arg(args, char *));
                break;
            case 'p':
                fprintf(f, "%p", va_arg(args, void *));
                break;
            case 'l':
                ++format;
                if (*format == 'f') {
                    fprintf(f, "%lf", va_arg(args, double));
                    ++format;
                }
            default:
                break;
            }
            ++format;
        } else {
            fputc(*format, f);
            ++format;
        }
    }
}

// priority - macros from sys/syslog.h with LOG prefix
// e.g. LOG_INFO LOG_ERR
void logging_format(int priority, const char *format, ...) {
    FILE *f = fopen(LOGGING_FILE, "a");
    if (f == NULL) {
        return;
    }
    va_list args;
    va_start(args, format);
    time_t now;

    time(&now);
    fprintf(f, "%s", ctime(&now));
    print_log_priority(f, priority);
    print_log_str_format(f, format, args);
    va_end(args);
    fclose(f);
}



