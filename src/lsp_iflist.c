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

#include "lsp_iflist.h"
#include "lsp_memory.h"

#include "string.h"

static lsp_list_head_t iflist = LSP_LIST_HEAD_INIT(iflist);

int lsp_iflist_add(lsp_interface_t *iface)
{
    lsp_list_add(&iface->list, &iflist);
    return LSP_ERR_NONE;
}

lsp_interface_t *lsp_iflist_iface_byname(const char *name)
{
    lsp_interface_t *iface;
    lsp_list_for(iface, list, &iflist)
    {
        if(strncasecmp(name, iface->ifname, sizeof(iface->ifname)) == 0)
            return iface;
    }
    return NULL;
}

lsp_interface_t *lsp_iflist_iface_byaddr(const lsp_addr_t addr)
{
    lsp_interface_t *iface;
    lsp_list_for(iface, list, &iflist)
    {
        if(addr == iface->dev_addr) return iface;
    }
    return NULL;
}
