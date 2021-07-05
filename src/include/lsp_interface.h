#ifndef LSP_INTERFACE_H
#define LSP_INTERFACE_H

#include <stddef.h>
#include "lsp_queue.h"
#include "lsp_mutex.h"

typedef struct lsp_interface lsp_interface_t;

typedef struct lsp_interface_ops
{
    int (*open)(lsp_interface_t *pv);
    int (*close)(lsp_interface_t *pv);
    int (*tx)(lsp_interface_t *pv, void *data, size_t len);
} lsp_interface_ops_t;

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

typedef struct lsp_interface
{
    int index;                   /** interface index (assigned on register) */
    char ifname[32];             /** interface name */
    int flags;                   /** interface flags. see #LSP_IF_FLAGS */
    uint32_t mtu;                /** max transmission unit of interface */
    lsp_interface_ops_t *ops;    /** interface functions */
    lsp_queue_handle_t rx_queue; /** interface rx queue */
    lsp_queue_handle_t tx_queue; /** interface tx queue */
    lsp_interface_stats_t stats; /** interface stats */
    void *interface_data;        /** interface data, used by driver (retrieve with interface_getdata()) */
} lsp_interface_t;

/**
 * @brief allocates memory for lsp_interface and initializes the queue
 * 
 * @param tx_queuelen max length of tx queue
 * @param rx_queuelen max length of rx queue
 * @param priv_len length of interface_data 
 * @param fmt format string for interface name
 * @param ... format args for interface name
 * @return lsp_interface_t* pointer to allocated memory on success, otherwise NULL
 */
lsp_interface_t *lsp_interface_alloc(int tx_queuelen,
                                     int rx_queuelen,
                                     size_t priv_len,
                                     const char *fmt, ...);

/**
 * @brief registers the interface to the system
 * 
 * @param iface pointer to interface
 * @return int #LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_interface_register(lsp_interface_t *iface);

/**
 * @brief Returns a pointer to interface data that can be used by interface drivers
 * 
 * @param iface pointer to interface
 * @return void* pointer to memory on success, otherwise NULL
 */
void *lsp_interface_getdata(lsp_interface_t *iface);

/**
 * @brief sends a packet to the system. 
 * 
 * @param iface pointer to source interface 
 * @param data pointer to packet
 * @param len length of packet in bytes
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_interface_qwrite(lsp_interface_t *iface, void *data, size_t len, int flags);

#endif