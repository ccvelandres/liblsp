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

#include "string.h"

static const char *tag = "lsp_port";

/** Port list */
static lsp_port_t *ports;

int lsp_port_init()
{
    LSP_ASSERT(ports == NULL, "%s: is called twice without free\n", __FUNCTION__);
    // max port + for any
    size_t blocksize = (LSP_PACKET_PORT_MAX + 2) * sizeof(lsp_port_t);
    ports = lsp_malloc(blocksize);
    if (ports == NULL)
    {
        lsp_verb(tag, "%s: could not allocate port resources\n", __FUNCTION__);
        return LSP_ERR_NOMEM;
    }
    memset(ports, 0, blocksize);
    lsp_verb(tag, "%s: allocated %d bytes for ports poolsize: %d connsize: %d\n",
             __FUNCTION__, blocksize, LSP_PACKET_PORT_MAX + 2, sizeof(lsp_port_t));

    // rely on calloc zero set the chunk and CONN_CLOSED is zero

    for (int i = 0; i < LSP_PACKET_PORT_MAX + 2; ++i)
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

lsp_port_t *lsp_port_get(uint8_t port)
{
    if (port > LSP_PACKET_PORT_MAX)
    {
        lsp_err(tag, "%s: invalid port number\n", __FUNCTION__);
        return NULL;
    }
    return &ports[port];
}

int lsp_listen(lsp_socket_t sock, int backlog)
{
    lsp_socket_t sk;
    uint8_t port = sock->attr.lport;

    if (port > LSP_PACKET_PORT_MAX)
    {
        lsp_err(tag, "%s: invalid port, call lsp_bind first or possible corruption\n", __FUNCTION__);
        return LSP_ERR_INVALID;
    }

    if (sock == NULL)
        return LSP_ERR_INVALID;

    sock->type = CONN_SERVER;
    sock->type = CONN_LISTEN;

    // allocate queue
    sock->children = lsp_queue_create(backlog, sizeof(lsp_socket_t));
    if (sock->children == NULL)
    {
        lsp_err(tag, "%s: could not create alloc mem for backlog\n", __FUNCTION__);
        return LSP_ERR_NOMEM;
    }

    if (lsp_list_is_empty(&ports[port].sockets))
    {
        lsp_list_add(&sock->portlist, &ports[port].sockets);
    }
    else
    {
        // insert after priority
        lsp_list_for(sk, portlist, &ports[port].sockets)
        {
            if (sk->attr.priority < sock->attr.priority)
            {
                lsp_list_insert_before(&sock->portlist, &sk->portlist);
                break;
            }
        }
    }

    lsp_info(tag, "%s: socket %p listening to port %u\n", __FUNCTION__, sock, sock->attr.lport);

    ports[port].state = PORT_OPEN;

    return LSP_ERR_NONE;
}

int lsp_bind(lsp_socket_t sock, lsp_sockaddr_t *sockaddr, size_t addrlen)
{
    int rc;
    if (sock == NULL)
        return LSP_ERR_INVALID;

    if (sockaddr->port == LSP_PORT_ANY)
        sock->attr.lport = LSP_PACKET_PORT_MAX + 1;
    else if (sockaddr->port > LSP_PACKET_PORT_MAX)
    {
        lsp_err(tag, "%s: lsp_bind invalid port %u, portrange: 0-%u + (LSP_PORT_ANY for default)\n", __FUNCTION__,
                sockaddr->port, LSP_PACKET_PORT_MAX);
        return LSP_ERR_PORT_INVALID;
    }

    if (ports[sock->attr.lport].state != PORT_CLOSED)
    {
        lsp_verb(tag, "%s: lsp_bind port %u is already in use\n", __FUNCTION__, sock->attr.lport);
        return LSP_ERR_PORT_IN_USE;
    }

    lsp_info(tag, "%s: binding socket %p to port %u\n", __FUNCTION__, sock, sock->attr.lport);

    return LSP_ERR_NONE;
}