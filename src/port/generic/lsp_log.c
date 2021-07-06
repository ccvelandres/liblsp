#include "lsp_log.h"

#include <stdarg.h>
#include <stdio.h>

void _lsp_hexdump(const void *ptr, size_t len, const char *fmt, ...)
{
   va_list args;
   
   va_start(args, fmt);
   vprintf(fmt, args);
   va_end(args);
    
    for(int i = 0; i < len; i+=sizeof(void*))
    {
        printf("%p  %04X:  ", (void*)(ptr + i), i);
        for(int j = 0; j < sizeof(void*); ++j)
            printf("%02X ", *(unsigned char*)(ptr + i + j));
        printf("\n");
    }
}

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