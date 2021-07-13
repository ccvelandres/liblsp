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

#ifndef LSP_ROUTING_H
#define LSP_ROUTING_H

#include <stddef.h>
#include "lsp_types.h"
#include "lsp_interface.h"
#include "lsp_list.h"

/** TODO: Add support for hops/mesh */
#define LSP_ROUTING_HOPS_ENABLED 0

typedef struct lsp_route_s
{
    lsp_list_t rlist; /** linked list for rtable */
    lsp_interface_t *iface; /** interface to route packet */
    lsp_addr_t addr; /** address of device */
    uint32_t linkspeed; /** linkspeed of hop in bytes/s */
    uint32_t timestamp; /** last discovery of route */
#if (LSP_ROUTING_HOPS_ENABLED)
    int numHops; 
    lsp_hop_t *hops;
#endif
}lsp_route_t;

/**
 * @brief Initializes the LSP Routing Module
 * 
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_routing_init();

/**
 * @brief Adds a new route if a route to addr does not exist yet
 * 
 * @param iface pointer to interface
 * @param addr connected node address
 * @param linkspeed linkspeed of route, -1 for unknown
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_route_add(lsp_interface_t* iface, lsp_addr_t addr, int linkspeed);

/**
 * @brief Look for interface to address
 * 
 * @param addr address
 * @return lsp_route_t* pointer to route
 */
lsp_interface_t *lsp_route_find(lsp_addr_t addr);

#endif