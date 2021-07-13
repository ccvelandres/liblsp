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

#include "lsp.h"
#include "lsp_routing.h"
#include "lsp_memory.h"
#include "lsp_log.h"
#include "lsp_list.h"
#include "lsp_time.h"

#include "string.h"

static const char *tag = "lsp_routing";

static lsp_list_head_t rtable = LSP_LIST_HEAD_INIT(rtable);

/** TODO: replace this with an MSRW lock */
static lsp_mutex_t rtable_mutex;

int lsp_routing_init()
{
    // for now mutex only needs initialization
    return lsp_mutex_init(&rtable_mutex);
}

int lsp_route_add(lsp_interface_t *iface, lsp_addr_t addr, int linkspeed)
{
    size_t blocksize = sizeof(lsp_route_t);
    lsp_route_t *route = NULL;

    // check if route already exist for this addr
    lsp_list_for(route, rlist, &rtable)
    {
        if (route->addr == addr)
            break;
    }

    if (route == NULL)
    {
        route = lsp_malloc(blocksize);
        if (route == NULL)
            return LSP_ERR_NOMEM;

        route->iface = iface;
        route->addr = addr;
        route->linkspeed;
        route->timestamp = lsp_gettime_ms();

        lsp_mutex_lock(&rtable_mutex, LSP_TIMEOUT_MAX);
        lsp_list_add(&route->rlist, &rtable);
        lsp_mutex_unlock(&rtable_mutex);

        lsp_verb(tag, "%s: route for %04X added via %s\n",
                 __FUNCTION__, addr, iface->ifname);
    }
    else
    {
        // replace route if linkspeed is better
        if (linkspeed < route->linkspeed)
        {
            lsp_verb(tag, "%s: route for %04X replaced via %s\n",
                     __FUNCTION__, addr, iface->ifname);

            lsp_mutex_lock(&rtable_mutex, LSP_TIMEOUT_MAX);
            route->iface = iface;
            route->linkspeed = linkspeed;
            route->timestamp = lsp_gettime_ms();
            lsp_mutex_unlock(&rtable_mutex);
        }
    }
    return LSP_ERR_NONE;
}

lsp_interface_t *lsp_route_find(lsp_addr_t addr)
{
    lsp_interface_t *iface = NULL;
    lsp_route_t *route;

    lsp_list_for(route, rlist, &rtable)
    {
        if (route->addr == addr)
            return route->iface;
    }

    return iface;
}