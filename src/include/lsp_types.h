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

#ifndef LSP_TYPES_H
#define LSP_TYPES_H

#include <stddef.h>
#include <stdint.h>

#include "lsp_defaults.h"

#define AF_LSP (46)
#define LSP_SOCK_STREAM (1)
#define LSP_SOCK_RAW (3)
#define LSPPROTO_LSP (46)
#define LSP_PORT_ANY (255)
#define LSP_ADDR_ANY (65535)
#define LSP_CONN_PRIO_MAX 7
#define LSP_CONN_PRIO_DEF 4

#if (LSP_POSIX)
#define LSP_TIMEOUT_MAX UINT32_MAX
#endif

/** Reserved ports for LSP services */
typedef enum
{
   LSP_SP_SYS = 0,        /** LSP System Management*/
   LSP_SP_PING = 1,       /** LSP Ping service */
   LSP_SP_FORWARDING = 2, /** LSP Forwarding Service */
   LSP_SP_MAX = 8         /** LSP Max service ports*/
} lsp_service_ports_t;

/**
   @defgroup LSP_ERR LSP Error codes
   @{
*/
#define LSP_ERR_NONE 0            /** Success */
#define LSP_ERR 1                 /** Generic error code */
#define LSP_ERR_NOMEM 2           /** Insufficient resources */
#define LSP_ERR_MUTEX 3           /** Mutex error */
#define LSP_ERR_TIMEOUT 4         /** Timeout */
#define LSP_ERR_INVALID 5         /** Invalid argument */
#define LSP_ERR_RESOURCE_IN_USE 5 /** Resource is in use */

#define LSP_ERR_QUEUE_FULL 10  /** Queue full */
#define LSP_ERR_QUEUE_EMPTY 11 /** Queue empty */

#define LSP_ERR_PORT_IN_USE 20  /** Port is in use */
#define LSP_ERR_PORT_INVALID 21 /** Invalid port number */

#define LSP_ERR_ADDR_NOTFOUND 30 /** Address not found */
#define LSP_ERR_ADDR_INVALID 31  /** Invalid address */

#define LSP_ERR_SOCK_NOT_CONNECTED 40 /** Socket is not connected */
#define LSP_ERR_SOCK_OPT_INVALID 41   /** Invalid sock opt arguments */

#define LSP_ERR_CONN_FULL 40 /** Connection pool is full */
/**@}*/

/**
   @defgroup LSP_IF_FLAGS LSP Interface Flags codes
   @{
*/
#define LSP_IF_FLAGS_ZERO_COPY (1 << 0)
#define LSP_IF_FLAGS_HW_CRC (1 << 1)
/**@}*/

/**
   @defgroup LSP_EVENT LSP Event codes
   @{
*/
#define LSP_EVENT_RX_EVENT 1
/**@}*/

/**
   @defgroup LSP_PACKET LSP Packet defines
   @{
*/
#define LSP_PACKET_ADDR_BITS 16
#define LSP_PACKET_PLEN_BITS 10
#define LSP_PACKET_PROTO_BITS 6
#define LSP_PACKET_FRAG_BITS 3
#define LSP_PACKET_SEQNUM_BITS 3
#define LSP_PACKET_PORT_BITS 5

#define LSP_PACKET_ADDR_MAX ((1 << LSP_PACKET_ADDR_BITS) - 1)
#define LSP_PACKET_PLEN_MAX ((1 << LSP_PACKET_PLEN_BITS) - 1)
#define LSP_PACKET_PROTO_MAX ((1 << LSP_PACKET_PROTO_BITS) - 1)
#define LSP_PACKET_FRAG_MAX ((1 << LSP_PACKET_FRAG_BITS) - 1)
#define LSP_PACKET_SEQNUM_MAX ((1 << LSP_PACKET_SEQNUM_BITS) - 1)
#define LSP_PACKET_PORT_MAX ((1 << LSP_PACKET_PORT_BITS) - 1)
/**@}*/

/** LSP typedef for address */
typedef uint16_t lsp_addr_t;

/** LSP typedef for port */
typedef uint8_t lsp_port_t;

/** Forward declaration for conf structure */
typedef struct lsp_conf_s lsp_conf_t;

/** Forward declaration for packet structure */
typedef struct lsp_packet_s lsp_packet_t;

/** Forward declaration for lsp buffer structure */
typedef struct lsp_buffer_s lsp_buffer_t;

/** Forward declaration for connection structure */
typedef struct lsp_conn_s lsp_conn_t;

/** Forward declaration for connection attribute structure */
typedef struct lsp_connattr_s lsp_connattr_t;

/** Forward declaration for socket structure */
typedef struct lsp_conn_s *lsp_socket_t;

/** Forward declaration for socket address structure */
typedef struct lsp_connadddr_s lsp_sockaddr_t;

#ifndef container_of
#define container_of(ptr, type, member) ({ (type *)((char *)ptr - offsetof(type, member)); })
#endif

#ifndef IS_ALIGNED
#define IS_ALIGNED(x) (x % sizeof(void *) == 0 || x % sizeof(int) == 0 || x == 1 || x == 2 ? 1 : 0)
#endif

#ifndef ALIGNED_SIZE
#define ALIGNED_SIZE(x) ((((x)) / sizeof(void *) + (x % sizeof(void *) ? 1 : 0)) * sizeof(void *))
#endif

#ifndef ALIGNED_SIZEOF
#define ALIGNED_SIZEOF(x) ALIGNED_SIZE(sizeof(x))
#endif

#endif