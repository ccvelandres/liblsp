#ifndef LSP_TYPES_H
#define LSP_TYPES_H

#include <stddef.h>
#include <stdint.h>

#include "lsp_defaults.h"

/**
   @defgroup LSP_ERR LSP Error codes
   @{
*/
#define LSP_ERR_NONE 0    /** Success */
#define LSP_ERR 1         /** Generic error code */
#define LSP_ERR_NOMEM 2   /** Insufficient resources */
#define LSP_ERR_MUTEX 3   /** Mutex error */
#define LSP_ERR_TIMEOUT 4 /** Timeout */
#define LSP_ERR_INVALID 5 /** Invalid argument */

#define LSP_ERR_QUEUE_FULL 10  /** Queue full */
#define LSP_ERR_QUEUE_EMPTY 11 /** Queue empty */
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
#define LSP_PACKET_PLEN_BITS 10
#define LSP_PACKET_PROTO_BITS 6
#define LSP_PACKET_FRAG_BITS 3
#define LSP_PACKET_SEQNUM_BITS 3
#define LSP_PACKET_SRCPORT_BITS 5
#define LSP_PACKET_DSTPORT_BITS 5

#define LSP_PACKET_PLEN_MAX ((1 << LSP_PACKET_PLEN_BITS) - 1)
#define LSP_PACKET_PROTO_MAX ((1 << LSP_PACKET_PROTO_BITS) - 1)
#define LSP_PACKET_FRAG_MAX ((1 << LSP_PACKET_FRAG_BITS) - 1)
#define LSP_PACKET_SEQNUM_MAX ((1 << LSP_PACKET_SEQNUM_BITS) - 1)
#define LSP_PACKET_SRCPORT_MAX ((1 << LSP_PACKET_SRCPORT_BITS) - 1)
#define LSP_PACKET_DSTPORT_MAX ((1 << LSP_PACKET_DSTPORT_BITS) - 1)
/**@}*/

/** LSP typedef for address */
typedef uint16_t lsp_addr_t; 

/** Forward declaration for packet structure */
typedef struct lsp_packet_s lsp_packet_t;

/** Forward declaration for lsp buffer structure */
typedef struct lsp_buffer_s lsp_buffer_t;

#ifndef container_of
#define container_of(ptr, type, member) ({ (type *)((char *)ptr - offsetof(type, member)); })
#endif

#ifndef ALIGNED_SIZE
#define ALIGNED_SIZE(x) ((((x))/sizeof(void*) + 1) * sizeof(void*))
#endif

#ifndef ALIGNED_SIZEOF
#define ALIGNED_SIZEOF(x) ALIGNED_SIZE(sizeof(x))
#endif

#endif