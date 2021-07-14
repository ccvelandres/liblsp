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

#include "lsp_memory.h"

#include <stdlib.h>

#if (DEBUG)
#include <pthread.h>
#include "lsp_list.h"
#include <stdint.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct block {
    lsp_list_t list;
    size_t size;
    void * ptr;
};
uint32_t allocBytes;
lsp_list_head_t head = LSP_LIST_HEAD_INIT(head);

void lsp_printmem()
{
    printf("Amount of allocated memory: %u bytes\n", allocBytes);
}
#endif

void *lsp_malloc(size_t len)
{
#if (DEBUG)
    struct block *b = malloc(sizeof(struct block));
    b->ptr = malloc(len);
    b->size = len;
    lsp_list_add(&b->list, &head);
    allocBytes += b->size;
    return b->ptr;
#else
    return malloc(len);
#endif
}

void *lsp_calloc(size_t nmemb, size_t size)
{
#if (DEBUG)
    struct block *b = malloc(sizeof(struct block));
    b->ptr = calloc(nmemb, size);
    b->size = nmemb * size;
    lsp_list_add(&b->list, &head);
    allocBytes += b->size;
    return b->ptr;
#else
    return calloc(nmemb, size);
#endif
}

void lsp_free(void *ptr)
{
#if (DEBUG)
    struct block *b = NULL;
    lsp_list_for(b, list, &head)
    {
        if(b->ptr == ptr)
        {
            allocBytes -= b->size;
            free(b);
        }
    }
#endif
    free(ptr);
}

