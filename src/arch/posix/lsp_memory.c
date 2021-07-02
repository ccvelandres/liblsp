#include "lsp_memory.h"

#include <stdlib.h>

void *lsp_malloc(size_t len)
{
    return malloc(len);
}

void *lsp_calloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}

void lsp_free(void *ptr)
{
    free(ptr);
}