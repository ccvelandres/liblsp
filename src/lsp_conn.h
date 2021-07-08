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

#ifndef LSP_SOCK_H
#define LSP_SOCK_H

#include <stddef.h>
#include "lsp_types.h"
#include "lsp_list.h"
#include "lsp_queue.h"

/** LSP Connection types */
typedef enum lsp_conn_type_e
{
    CONN_CLIENT,
    CONN_SERVER,
    CONN_CHILD
} lsp_conn_type_t;

/** LSP Connection states */
typedef enum lsp_conn_state_e
{
    CONN_FREE = 0,
    CONN_CLOSED,
    CONN_OPEN,
    CONN_LISTEN,
    CONN_CONNECTED
} lsp_conn_state_t;

/** LSP Socket options */
typedef enum lsp_sockopt_e
{
    LSP_SOCKOPTS_SETPRIO /** Set the socket priority */
} lsp_sockopt_t;

/** LSP Connection struct */
struct lsp_conn_s
{
    lsp_conn_type_t type;   /** Connection type (Client or Server) and (Local or IPC) */
    lsp_conn_state_t state; /** Connection state */
    union
    {
        lsp_list_head_t child; /** child connections */
        lsp_conn_t *parent;     /** parent connection */
    };
    lsp_port_t port;             /** Connection port */
    uint32_t timestamp;          /** Time the connection was opened */
    lsp_queue_handle_t rx_queue; /** Queue for connection packets */
};

/** LSP Connection address struct */
struct lsp_connadddr_s
{
    lsp_port_t port;
    lsp_addr_t addr;
};

/**
 * @brief Initializes LSP Connection module
 * 
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_conn_init();

/**
 * @brief allocate new connection
 * 
 * @param type type
 * @return lsp_conn_t* pointer on success, otherwise NULL
 */
lsp_conn_t *lsp_conn_alloc(lsp_conn_type_t type);

/**
 * @brief closes the connection
 * 
 * @param conn connection
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_conn_close(lsp_conn_t *conn);

/**
 * @brief Push data to connection
 * 
 * @param conn connection
 * @param buffer buffer
 * @return int 
 */
int lsp_conn_rxq(lsp_conn_t *conn, lsp_buffer_t *buffer);

#endif