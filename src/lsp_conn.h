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
    CONN_SERVER
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

/** LSP Connection address struct */
struct lsp_connadddr_s
{
    uint8_t port;
    lsp_addr_t addr;
};

/** LSP Connection attributes */
struct lsp_connattr_s
{
    uint8_t priority; /** Connection priority */
    uint8_t sport; /** Source port */
    lsp_addr_t saddr; /** Source address */
    uint8_t rport;  /** Remote Port */
    lsp_addr_t raddr; /** Remote Address */
    uint32_t flags; /** Connection Flags */
};

/** LSP Connection struct */
struct lsp_conn_s
{
    lsp_conn_type_t type;   /** Connection type (Client or Server) and (Local or IPC) */
    lsp_conn_state_t state; /** Connection state */
    lsp_connattr_t attr; /** Connection attributes */
    lsp_list_t portlist; /** linked list for port connections */
    union {
        lsp_list_head_t children; /** linked list for child connections */
        lsp_conn_t *parent;   /** parent connection */
    };
    uint8_t port;             /** Connection port */
    uint32_t timestamp;          /** Time the connection was opened */
    lsp_queue_handle_t rx_queue; /** Queue for connection packets */
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
 * @brief frees the connection and returns it to the pool.
 * Internally calls lsp_conn_close if the connection is not yet closed
 * 
 * @param conn connection
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_conn_free(lsp_conn_t *conn);

/**
 * @brief flushes the rx queue of the connection
 * 
 * @param conn connection
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_conn_rxq_flush(lsp_conn_t *conn);

/**
 * @brief Push data to connection
 * 
 * @param conn connection
 * @param buffer buffer
 * @return int 
 */
int lsp_conn_rxq_push(lsp_conn_t *conn, lsp_buffer_t *buffer);

#endif