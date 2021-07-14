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

#if (DEBUG)
/**
 * @brief prints amount of allocated mem to console ifsupported
 * 
 * @return void 
 */
void lsp_printmem();
#endif


#endif