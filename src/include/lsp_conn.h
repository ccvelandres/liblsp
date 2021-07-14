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
#include "lsp_egroup.h"

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

/** LSP Connection events */
typedef enum lsp_conn_events_e
{
    CONN_EV_RECEIVE = 0x01,
    CONN_EV_SEND = 0x02,
    CONN_EV_BOUND = 0x04,
    CONN_EV_LISTEN = 0x08,
    CONN_EV_ACCEPT = 0x10,
    CONN_EV_CONNECT = 0x20,
    CONN_EV_CLOSED = 0x40,
    CONN_EV_INTR = 0x80,
    CONN_EV_ALL = 0xFF
}lsp_conn_events_t;

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
    uint8_t lport;    /** Local port */
    lsp_addr_t laddr; /** Local address */
    uint8_t rport;    /** Remote Port */
    lsp_addr_t raddr; /** Remote Address */
    uint32_t flags;   /** Connection Flags */
};

/** LSP Connection struct */
struct lsp_conn_s
{
    lsp_conn_type_t type;       /** Connection type (Client or Server) and (Local or IPC) */
    lsp_conn_state_t state;     /** Connection state */
    lsp_connattr_t attr;        /** Connection attributes */
    lsp_egroup_handle_t egroup; /** Event Group Handle*/
    lsp_list_t portlist;        /** linked list for port connections */
    union
    {
        lsp_queue_handle_t children; /** queue for child connections */
        lsp_conn_t *parent;          /** parent connection */
    };
    uint32_t rcv_timeout;        /** Max receive timeout */
    uint32_t snd_timeout;        /** Max send timeout */
    uint32_t s_opt;              /** Socket options */
    uint32_t timestamp;          /** Time the connection was opened */
    lsp_queue_handle_t rx_queue; /** primitive for sync TODO: implement something like event groups or cond var */
    lsp_list_head_t rxstream, txstream;
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
 * @return lsp_conn_t* pointer on success, otherwise NULL
 */
lsp_conn_t *lsp_conn_alloc();

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