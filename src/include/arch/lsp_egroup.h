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

#ifndef LSP_EGROUP_H
#define LSP_EGROUP_H

#include <stddef.h>
#include <stdint.h>

#include "lsp_types.h"

#ifdef LSP_POSIX
#include <pthread.h>

typedef void * lsp_egroup_handle_t;
typedef uint32_t lsp_egroup_bits_t;

#else
typedef void *lsp_egroup_handle_t;
#endif

/**
 * @brief creates an event group 
 * 
 * @return lsp_egroup_handle_t 
 */
lsp_egroup_handle_t lsp_egroup_create();

/**
 * @brief read bits in event group and/or optionally block until bits are set
 * 
 * @param handle event group
 * @param bits bitmask for bits to wait
 * @param clearOnExit clears all bits if true on return
 * @param waitAll waits for all bits to be set
 * @param timeout timeout in ms
 * @return lsp_egroup_bits_t value of bits being waited for became set or 0 if timeout expired
 */
lsp_egroup_bits_t lsp_egroup_wait(lsp_egroup_handle_t handle, lsp_egroup_bits_t bits, int clearOnExit, int waitAll, uint32_t timeout);

/**
 * @brief sets bits within an event group
 * 
 * @param handle event group
 * @param bits bitmask for bits to set
 * @return lsp_egroup_bits_t value of event bits on return from this function
 */
lsp_egroup_bits_t lsp_egroup_set(lsp_egroup_handle_t handle, lsp_egroup_bits_t bits);

#endif