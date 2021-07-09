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
#include "lsp_port.h"
#include "lsp_memory.h"
#include "lsp_conn.h"
#include "lsp_log.h"

static const char *tag = "lsp_port";

/** Port list */
static lsp_port_t *ports;

int lsp_port_init()
{
    LSP_ASSERT(ports == NULL, "lsp_port_init() is called twice without free\n");
    // max port + for any
    size_t blocksize = (LSP_PACKET_PORT_MAX + 2) * sizeof(lsp_port_t);
    ports = lsp_malloc(blocksize);
    if(ports == NULL)
    {
        lsp_verb(tag, "could not allocate port resources\n");
        return -LSP_ERR_NOMEM;
    }
    memset(ports, 0, blocksize);
    lsp_verb(tag, "lsp_port_init allocated %d bytes for ports poolsize: %d connsize: %d\n", blocksize, LSP_PACKET_PORT_MAX + 2, sizeof(lsp_port_t));

    // rely on calloc zero set the chunk and CONN_CLOSED is zero

    for(int i = 0; i < LSP_PACKET_PORT_MAX + 2; ++i)
    {
        lsp_list_head_init(&ports[i].sockets);
    }

    return LSP_ERR_NONE;
}

int lsp_port_free()
{
    lsp_free(ports);
    ports = NULL;
    return LSP_ERR_NONE;
}

int lsp_listen(lsp_socket_t sock, int backlog)
{
    lsp_socket_t sk;
    uint8_t port = (sock->port < LSP_PACKET_PORT_MAX ? sock->port : LSP_PACKET_PORT_MAX);

    if(sock == NULL)
        return LSP_ERR_INVALID;
        
    sock->type = CONN_SERVER;
    sock->type = CONN_LISTEN;

    if(lsp_list_is_empty(&ports[port].sockets))
    {
        lsp_list_add(&sock->portlist, &ports[port].sockets);
    }
    else {
        // insert after priority
        lsp_list_for(sk, portlist, &ports[port].sockets)
        {
            if(sk->attr.priority < sock->attr.priority)
            {
                lsp_list_insert_before(&sock->portlist, &sk->portlist);
                break;
            }
        }
    }

    lsp_info(tag, "socket %p listening to port %u\n", sock, sock->port);

    ports[port].state = PORT_OPEN;

    return LSP_ERR_NONE;   
}

int lsp_bind(lsp_socket_t sock, lsp_sockaddr_t *sockaddr, size_t addrlen)
{
    int rc;
    if(sock == NULL)
        return LSP_ERR_INVALID;

    if(sockaddr->port == LSP_PORT_ANY)
        sock->port = LSP_PACKET_PORT_MAX + 1;
    else if(sockaddr->port > LSP_PACKET_PORT_MAX)
    {
        lsp_err(tag, "lsp_bind invalid port %u, portrange: 0-%u + (LSP_PORT_ANY for default)\n",
            sockaddr->port, LSP_PACKET_PORT_MAX);
        return LSP_ERR_PORT_INVALID;
    }

    if(ports[sock->port].state != PORT_CLOSED)
    {
        lsp_verb(tag, "lsp_bind port %u is already in use\n", sock->port);
        return LSP_ERR_PORT_IN_USE;
    }

    lsp_info(tag, "binding socket %p to port %u\n", sock, sock->port);

    return LSP_ERR_NONE;    
}