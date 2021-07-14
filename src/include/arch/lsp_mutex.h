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

#ifndef LSP_MUTEX_H
#define LSP_MUTEX_H

#include <stddef.h>
#include <stdint.h>

#include "lsp_types.h"

#ifdef LSP_POSIX
#include <pthread.h>

/**
 * Platform speific typedef for mutex
 */
typedef pthread_mutex_t lsp_mutex_t;

#else
typedef void lsp_mutex_t;
#endif

/**
 * @brief lsp wrapper for creating dynamic mutexes
 * 
 * @param mutex pointer to mutex handle
 * @return int #LSP_ERR_NONE on success, otherwise #LSP_ERR_MUTEX
 */
lsp_mutex_t *lsp_mutex_create();

/**
 * @brief lsp wrapper for initializing static mutexes
 * 
 * @param mutex pointer to mutex handle
 * @return int #LSP_ERR_NONE on success, otherwise #LSP_ERR_MUTEX
 */
int lsp_mutex_init(lsp_mutex_t *mutex);

/**
 * @brief lsp wrapper for destroying mutexes
 * 
 * @param mutex pointer to mutex handle
 * @return int #LSP_ERR_NONE on success, otherwise #LSP_ERR_MUTEX
 */
int lsp_mutex_destroy(lsp_mutex_t *mutex);

/**
 * @brief lsp wrapper for locking mutexes
 * 
 * @param mutex pointer to mutex handle
 * @param timeout timeout in ms, LSP_TIMEOUT_MAX to wait forever
 * @return int #LSP_ERR_NONE on success, otherwise #LSP_ERR_MUTEX or #LSP_ERR_TIMEOUT
 */
int lsp_mutex_lock(lsp_mutex_t *mutex, uint32_t timeout);

/**
 * @brief lsp wrapper for unlocking mutexes
 * 
 * @param mutex pointer to mutex handle
 * @return int #LSP_ERR_NONE on success, otherwise #LSP_ERR_MUTEX
 */
int lsp_mutex_unlock(lsp_mutex_t *mutex);

#endif