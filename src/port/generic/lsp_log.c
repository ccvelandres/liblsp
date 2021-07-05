#include "lsp_log.h"

#include <stdarg.h>
#include <stdio.h>


void _lsp_log(int logl, const char *fmt, ...)
{
   va_list args;
   
   va_start(args, fmt);
   vprintf(fmt, args);
   va_end(args);
}

void _lsp_flush()
{
    fflush(stdout);
    fflush(stderr);
}

void _lsp_abort()
{
    while(1);
}