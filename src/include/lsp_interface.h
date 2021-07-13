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

#ifndef LSP_INTERFACE_H
#define LSP_INTERFACE_H

#include <stddef.h>
#include "lsp_queue.h"
#include "lsp_mutex.h"
#include "lsp_list.h"
#include "lsp_types.h"

/** Forward declaration for lsp_interface_s */
typedef struct lsp_interface_s lsp_interface_t;

/** LSP Interface operation functions*/
typedef struct lsp_interface_ops
{
    int (*open)(lsp_interface_t *pv);                       /** called by system to initialize interface */
    int (*close)(lsp_interface_t *pv);                      /** called by system during shutdown */
    int (*tx)(lsp_interface_t *pv, void *data, size_t len); /** used by system to transmit packets */
} lsp_interface_ops_t;

/** LSP Interface stats for monitoring*/
typedef struct lsp_interface_stats
{
    uint32_t tx_count; /** total transmitted packet count */
    uint32_t rx_count; /** total received packet count */
    uint32_t tx_bytes; /** total transmitted byte count */
    uint32_t rx_bytes; /** total received byte count */
    uint32_t dropped;  /** total dropped packet count */
    uint32_t tx_error; /** total transmit errors */
    uint32_t rx_error; /** total receive errors */
} lsp_interface_stats_t;

/** LSP Interface main structure */
struct lsp_interface_s
{
    int index;                   /** interface index (assigned on register) */
    char ifname[32];             /** interface name */
    lsp_addr_t dev_addr;         /** interface address, set by system to system address */
    int flags;                   /** interface flags. see #LSP_IF_FLAGS */
    uint32_t mtu;                /** max transmission unit of interface */
    lsp_interface_ops_t *ops;    /** interface functions */
    lsp_interface_stats_t stats; /** interface stats */
    int min_header_len;          /** minimum header len to allocate in front of lsp packet for encapsulation */
    lsp_list_t list;             /** interface is implemented as linked list*/
    lsp_queue_handle_t tx_queue; /** interface tx queue */
    void *interface_data;        /** interface data, used by driver (retrieve with interface_getdata()) */
};

/**
 * @brief allocates memory for lsp_interface and initializes the queue
 * 
 * @param tx_queuelen max length of tx queue
 * @param priv_len length of interface_data 
 * @param fmt format string for interface name
 * @param ... format args for interface name
 * @return lsp_interface_t* pointer to allocated memory on success, otherwise NULL
 */
lsp_interface_t *lsp_interface_alloc(int tx_queuelen,
                                     size_t priv_len,
                                     const char *fmt, ...);

/**
 * @brief Returns a pointer to interface data that can be used by interface drivers
 * 
 * @param iface pointer to interface
 * @return void* pointer to memory on success, otherwise NULL
 */
void *lsp_interface_getdata(lsp_interface_t *iface);

/**
 * @brief sends a packet to the system. 
 * @details Ideally, interface should go with zero-copy approach with lsp_buffer_t.
 * This function is for for the sake of simple testing and reference for sending packets to 
 * the system
 * 
 * @param iface pointer to source interface 
 * @param data pointer to packet
 * @param len length of packet in bytes
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_interface_qwrite(lsp_interface_t *iface, void *data, size_t len, int flags);

#endif