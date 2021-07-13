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

#ifndef LSP_PORT_H
#define LSP_PORT_H

#include <stddef.h>
#include "lsp_types.h"
#include "lsp_list.h"
#include "lsp_queue.h"

/** LSP Port state */
typedef enum lsp_port_state_e
{
    PORT_CLOSED,
    PORT_OPEN
}lsp_port_state_t;

/** LSP Port struct */
typedef struct lsp_port_s
{
    lsp_port_state_t state; /** port state */
    lsp_list_head_t sockets; /** socket list */
}lsp_port_t;

/**
 * @brief Initializes LSP Port Module
 * 
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_port_init();

/**
 * @brief Frees allocated resources for LSP Port Module
 * 
 * @return int LSP_ERR_NONE on sucecss, otherwise an error code
 */
int lsp_port_free();

#endif