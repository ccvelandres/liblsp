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

#ifndef LSP_QUEUE_H
#define LSP_QUEUE_H

#include <stddef.h>

#include "lsp_types.h"

typedef void * lsp_queue_handle_t;

/**
 * @brief create a queue
 * 
 * @param len max len of queue
 * @param itemsize size of item in bytes
 * @return lsp_queue_handle_t* 
 */
lsp_queue_handle_t lsp_queue_create(int len, size_t itemsize);
/**
 * @brief destroys the queue
 * 
 * @param handle pointer to queue handle
 */
void lsp_queue_destroy(lsp_queue_handle_t handle);

/**
 * @brief pushes data to queue
 * 
 * @param handle pointer to queue handle
 * @param data pointer to data
 * @param timeout timeout in ms, -1 to block
 * @return int #LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_queue_push(lsp_queue_handle_t handle, const void *const data, const int timeout);

/**
 * @brief pops data from queue
 * 
 * @param handle pointer to queue handle
 * @param data pointer to pointer
 * @param timeout timeout in ms, -1 to block
 * @return int #LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_queue_pop(lsp_queue_handle_t handle, void *data, const int timeout);

/**
 * @brief returns the length of queue
 * 
 * @param handle pointer to queue handle
 * @return int length of queue
 */
int lsp_queue_len(lsp_queue_handle_t handle);

/**
 * @brief returns item size of queue
 * 
 * @param handle pointer to queue handle
 * @return int length in bytes
 */
int lsp_queue_itemsize(lsp_queue_handle_t handle);
#endif