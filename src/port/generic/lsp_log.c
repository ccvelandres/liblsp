/* 
 * Copyright (c) 2021 Cedric Velandres
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Authors: 
 *      Cedric Velandres, <ccvelandres@gmail.com>
 */

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