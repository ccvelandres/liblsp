#ifndef LSP_BUFFER_H
#define LSP_BUFFER_H

#include <stddef.h>
#include "lsp_types.h"
#include "lsp_interface.h"

/** LSP Packet structure */
struct lsp_packet_s
{
    lsp_addr_t dst_addr;                         /** Destination address */
    lsp_addr_t src_addr;                         /** Source Address */
    uint16_t plen : LSP_PACKET_PLEN_BITS;        /** Payload Length */
    uint16_t proto : LSP_PACKET_PROTO_BITS;      /** Payload Protocol*/
    uint16_t frag : LSP_PACKET_FRAG_BITS;        /** Fragmentation */
    uint16_t seqnum : LSP_PACKET_SEQNUM_BITS;    /** Sequence Number */
    uint16_t src_port : LSP_PACKET_SRCPORT_BITS; /** Source Port */
    uint16_t dst_port : LSP_PACKET_DSTPORT_BITS; /** Destination Port */

    union
    {
        uint8_t pl8[0];   /** uint8_t payload cast */
        uint16_t pl16[0]; /** uint16_t payload cast */
        uint32_t pl32[0]; /** uint32_t payload cast*/
    };
};

/** LSP Packet Buffer structure */
struct lsp_buffer_s
{
    lsp_interface_t *iface;
    lsp_list_t list;
    size_t headroom, tailroom;
    unsigned char *data, *tail, *end;
    lsp_packet_t *lsp_packet;
    unsigned char *head;
};

/**
 * @brief allocates a buffer 
 * 
 * @param iface pointer to interface to send/receive
 * @param len 
 * @return lsp_buffer_t* 
 */
lsp_buffer_t *lsp_buffer_alloc(lsp_interface_t *iface, size_t len);

/**
 * @brief add data to the buffer
 * 
 * @param buff pointer to buffer descriptor
 * @param len length of data to add
 * @return void* pointer to first byte of added data, otherwise NULL on error
 */
void *lsp_buffer_put(lsp_buffer_t *buff, size_t len);

/**
 * @brief add data to front of buffer
 * 
 * @param buff pointer to buffer descriptor
 * @param len length of data to add
 * @return void* pointer to front of added data, otherwise NULL on error
 */
void *lsp_buffer_push(lsp_buffer_t *buff, size_t len);

/**
 * @brief frees the buffer
 * 
 * @param buff pointer to buffer
 */
void lsp_buffer_free(lsp_buffer_t *buff);

void lsp_buffer_debug(lsp_buffer_t *buff);

#endif