#ifndef LSP_INTERFACE_LIST_H
#define LSP_INTERFACE_LIST_H

#include <stddef.h>
#include "lsp_queue.h"
#include "lsp_mutex.h"
#include "lsp_interface.h"
#include "lsp_list.h"
#include "lsp_types.h"

/**
 * @brief registers the interface to iflist. Must only be called before starting the service, may cause UD when called while service is running
 * 
 * @param iface pointer to interface struct
 * @return int #LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_iflist_add(lsp_interface_t *iface);

/**
 * @brief search for an interface with matching iface name
 * 
 * @param name name to match
 * @return lsp_interface_t* pointer to interface on success, otherwise NULL
 */
lsp_interface_t *lsp_iflist_iface_byname(const char *name);

/**
 * @brief search for an interface with matching address
 * 
 * @param addr address to match
 * @return lsp_interface_t* pointer to interface on success, otherwise NULL
 */
lsp_interface_t *lsp_iflist_iface_byaddr(const lsp_addr_t addr);

#endif