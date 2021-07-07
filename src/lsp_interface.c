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

#include "lsp_interface.h"
#include "lsp_memory.h"
#include "lsp_log.h"

#include "stdarg.h"
#include "stdio.h"

static const char *tag = "lsp_interface";

lsp_interface_t *lsp_interface_alloc(int tx_queuelen, size_t priv_len, const char *fmt, ...)
{
    int rc = -LSP_ERR_NOMEM;
    lsp_interface_t *iface = lsp_calloc(1, sizeof(lsp_interface_t) + priv_len);
    if(iface == NULL) goto err;

    iface->tx_queue = lsp_queue_create(tx_queuelen);
    if(iface->tx_queue == NULL) goto txq_err;

    va_list args;
    va_start(args, fmt);
    rc = vsnprintf(iface->ifname, sizeof(iface->ifname), fmt, args);
    va_end(args);
    if(!(rc > 0 && rc < sizeof(iface->ifname)))
        lsp_warn(tag, "interface name might not be registered correctly: %s\n", iface->ifname);

    return iface;

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