#include "lsp_interface.h"
#include "lsp_memory.h"
#include "lsp_log.h"

#include "stdarg.h"
#include "stdio.h"

lsp_interface_t *lsp_interface_alloc(int tx_queuelen, int rx_queuelen, size_t priv_len, const char *fmt, ...)
{
    int rc = -LSP_ERR_NOMEM;
    lsp_interface_t *iface = lsp_calloc(1, sizeof(lsp_interface_t) + priv_len);
    if(iface == NULL) goto err;

    iface->tx_queue = lsp_queue_create(tx_queuelen);
    if(iface->tx_queue == NULL) goto txq_err;
    iface->rx_queue = lsp_queue_create(rx_queuelen);
    if(iface->rx_queue == NULL) goto rxq_err;

    va_list args;
    va_start(args, fmt);
    rc = vsnprintf(iface->ifname, sizeof(iface->ifname), fmt, args);
    va_end(args);
    if(!(rc > 0 && rc < sizeof(iface->ifname)))
        lsp_warn("interface name might not be registered correctly: %s\n", iface->ifname);

    return iface;

    lsp_free(iface->rx_queue);
rxq_err:
    lsp_free(iface->tx_queue);
txq_err:
    lsp_free(iface);
err:
    return rc;
}

int lsp_interface_register(lsp_interface_t *iface)
{

}


void *lsp_interface_getdata(lsp_interface_t *iface)
{
    return (iface->interface_data);
}

int lsp_interface_qwrite(lsp_interface_t *iface, void *data, size_t len, int flags)
{
    
}