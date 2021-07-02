#ifndef LSP_MEMORY_H
#define LSP_MEMORY_H

#include <stddef.h>

#include "lsp_types.h"

/**
 * @brief lsp wrapper for malloc
 * 
 * @param len length of memory to allocate in bytes
 * @return void* pointer to allocated memory, or NULL in failure
 */
void *lsp_malloc(size_t len);
/**
 * @brief lsp wrapper for calloc
 * 
 * @param nmemb number of members/chunks to allocate
 * @param len size of memory chunk (bytes)
 * @return void* pointer to allocated memory initialized to zero, or NULL in failure
 */
void *lsp_calloc(size_t nmemb, size_t size);
/**
 * @brief lsp wrapper for free
 * 
 */
void lsp_free(void *ptr);

#endif