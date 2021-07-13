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

#ifndef LSP_ROUTING_H
#define LSP_ROUTING_H

#include <stddef.h>
#include "lsp_types.h"
#include "lsp_list.h"

/**
 * @brief Starts the LSP Core Module
 * 
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_core_start();

/**
 * @brief Send an event to the core module
 * 
 * @param ev event id
 * @param data pointer to data
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_core_sendevent(lsp_events_t ev, void *data);

#endif