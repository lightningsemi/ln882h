#ifndef _WRAP_STDIO_H_
#define _WRAP_STDIO_H_

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*stdio_write_fn)(const char *buf, int len);

void __wrap_sprintf(stdio_write_fn write, const char *format, ...);
void __sprintf(const char *tag, stdio_write_fn write, const char *format, va_list args);

#ifdef __cplusplus
}
#endif
#endif /* _WRAP_STDIO_H_ */
