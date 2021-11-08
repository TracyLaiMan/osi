#include <stdio.h>

#define OS_GENERIC 1

#if defined(OS_GENERIC)

/* syslog didn't work well here since we would be redefining LOG_DEBUG. */
#include <stdio.h>

#define LOGWRAPPER(tag, fmt, args...) \
  fprintf(stderr, "%s: " fmt "\n", tag, ##args)

#define LOG_VERBOSE(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_DEBUG(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_INFO(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_WARN(...) LOGWRAPPER(__VA_ARGS__)
#define LOG_ERROR(...) LOGWRAPPER(__VA_ARGS__)

#define LOG_EVENT_INT(...)

#endif /* !defined(OS_GENERIC) */

int main(int argc, char **argv){
    LOG_DEBUG("Tracy","test");
}