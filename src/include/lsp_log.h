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
#define LSP_LOGL LSP_LOGL_INFO
#endif

#ifndef lsp_fmt
#define lsp_fmt(fmt) #fmt
#endif

void _lsp_log(const char *fmt, ...);
void _lsp_flush();
void _lsp_abort();

#define lsp_log(logl, fmt, ...) \
    if (LSP_LOGL >= logl)       \
    _lsp_log(fmt, ##__VA_ARGS__)

#define lsp_err(fmt, ...) lsp_log(LSP_LOGL_ERROR, lsp_fmt(fmt), ##__VA_ARGS__)
#define lsp_warn(fmt, ...) lsp_log(LSP_LOGL_WARNING, lsp_fmt(fmt), ##__VA_ARGS__)
#define lsp_info(fmt, ...) lsp_log(LSP_LOGL_INFO, lsp_fmt(fmt), ##__VA_ARGS__)
#define lsp_dbg(fmt, ...) lsp_log(LSP_LOGL_DEBUG, lsp_fmt(fmt), ##__VA_ARGS__)
#define lsp_verb(fmt, ...) lsp_log(LSP_LOGL_VERBOSE, lsp_fmt(fmt), ##__VA_ARGS__)

#define LSP_ASSERT(cond, msg, ...)                                  \
    if (!cond)                                                       \
    {                                                               \
        _lsp_log("ASSERT FAIL: \"" #cond "\" " msg, ##__VA_ARGS__); \
        _lsp_flush();                                               \
        _lsp_abort();                                               \
    }

#endif