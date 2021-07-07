#ifndef LSP_LOG_H
#define LSP_LOG_H

#include <stddef.h>

#include "lsp_types.h"

#define LSP_LOGL_NONE 0
#define LSP_LOGL_ERROR 1
#define LSP_LOGL_WARNING 2
#define LSP_LOGL_INFO 3
#define LSP_LOGL_DEBUG 4
#define LSP_LOGL_VERBOSE 5

#ifndef LSP_LOGL
#define LSP_LOGL LSP_LOGL_VERBOSE
#endif

#define __STRING(x) #x
#define _STRING(x) __STRING(x)
#define STRING(x)

#ifndef lsp_fmt
#define lsp_fmt(fmt) fmt
#endif

void _lsp_hexdump(const void *ptr, size_t len, const char *fmt, ...);
void _lsp_log(int logl, const char *fmt, ...);
void _lsp_flush();
void _lsp_abort();

#define lsp_hexdump(ptr, len) \
    _lsp_hexdump(ptr, len, "hexdump @ %s:%d var:%s@%08x\n", __FILE__, __LINE__, #ptr, ptr)

#define lsp_log(logl, prefix, fmt, ...) \
    if (LSP_LOGL >= logl)               \
    _lsp_log(logl, prefix fmt, ##__VA_ARGS__)

#define lsp_err(tag, fmt, ...) lsp_log(LSP_LOGL_ERROR, "E: %s ", lsp_fmt(fmt), tag, ##__VA_ARGS__)
#define lsp_warn(tag, fmt, ...) lsp_log(LSP_LOGL_WARNING, "W: %s ", lsp_fmt(fmt), tag, ##__VA_ARGS__)
#define lsp_info(tag, fmt, ...) lsp_log(LSP_LOGL_INFO, "I: %s ", lsp_fmt(fmt), tag, ##__VA_ARGS__)
#define lsp_dbg(tag, fmt, ...) lsp_log(LSP_LOGL_DEBUG, "D: %s ", lsp_fmt(fmt), tag, ##__VA_ARGS__)
#define lsp_verb(tag, fmt, ...) lsp_log(LSP_LOGL_VERBOSE, "V: %s ", lsp_fmt(fmt), tag, ##__VA_ARGS__)

#define LSP_ASSERT(cond, msg, ...)                                  \
    if (!cond)                                                      \
    {                                                               \
        _lsp_log(LSP_LOGL_ERROR, "ASSERT FAIL: \"" #cond "\" " msg, ##__VA_ARGS__); \
        _lsp_flush();                                               \
        _lsp_abort();                                               \
    }

#endif