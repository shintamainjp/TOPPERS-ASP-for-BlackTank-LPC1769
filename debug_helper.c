
#include "debug_helper.h"

void svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
    if (ercd < 0) {
        t_perror(LOG_ERROR, file, line, expr, ercd);
    }
}

