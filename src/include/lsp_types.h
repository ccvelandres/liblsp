#ifndef LSP_TYPES_H
#define LSP_TYPES_H

#include <stddef.h>
#include <stdint.h>

#include "lsp_defaults.h"

/**
   @defgroup LSP_ERR LSP Error codes
   @{
*/
#define LSP_ERR_NONE 0 /** Success */
#define LSP_ERR 1 /** Generic error code */
#define LSP_ERR_NOMEM 2 /** Insufficient resources */
#define LSP_ERR_MUTEX 3 /** Mutex error */
#define LSP_ERR_TIMEOUT 4 /** Timeout */
#define LSP_ERR_INVALID 5 /** Invalid argument */

#define LSP_ERR_QUEUE_FULL 10 /** Queue full */
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

typedef uint16_t lsp_addr_t; /** LSP typedef for address */

/**
 * @brief LSP Packet
 * 
 * This structure provides easy access to fields in the lsp packet
 * 
 */
typedef struct lsp_packet
{
    lsp_addr_t dst_addr;
    lsp_addr_t src_addr;
    uint16_t plen:10;
    uint16_t l2proto:6;
    uint16_t frag:3;
    uint16_t seqnum:3;
    uint16_t src_port:5;
    uint16_t dst_port:5;

    union {
        uint8_t pl8[0];
        uint16_t pl16[0];
        uint32_t pl32[0];
    };
}lsp_packet_t;

#ifndef container_of
#define container_of(ptr, type, member) ({ \
    (type *)((char *)ptr - offsetof(type, member)); })
#endif

#endif