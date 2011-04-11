#ifndef DEBUG_HELPER_H
#define DEBUG_HELPER_H

#include <kernel.h>
#include <t_syslog.h>
#include <syssvc/syslog.h>

void svc_perror(const char *file, int_t line, const char *expr, ER ercd);

#define SVC_PERROR(expr) svc_perror(__FILE__, __LINE__, #expr, (expr))

#endif

