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

static const char *tag = "lsp_socket";

lsp_socket_t lsp_socket(int domain, int type, int protocol)
{
    lsp_socket_t sock = NULL;

    if(domain != AF_LSP){
        lsp_err(tag, "lsp_socket domain must be set to AF_LSP\n");
        return NULL;
    }

    if(type != LSP_SOCK_RAW && type != LSP_SOCK_STREAM)
    {
        lsp_err(tag, "lsp_socket type must be either LSP_SOCK_RAW or LSP_SOCK_STREAM\n");
        return NULL;
    }

    if(protocol != LSPPROTO_LSP)
    {
        lsp_err(tag, "lsp_socket protocol must be LSPPROTO_LSP\n");
        return NULL;
    }

    // assume server so head list is initialized
    sock = lsp_conn_alloc(CONN_SERVER);
    if(sock == NULL)
    {
        lsp_verb(tag, "lsp_socket failed to create new socket\n");
        goto end;
    }

end:
    return sock;
}

void lsp_closesocket(lsp_socket_t sock, int how)
{
    int rc;
    (void) how; // unused

    lsp_conn_free(sock);
}


lsp_socket_t lsp_accept(lsp_socket_t sock, uint32_t timeout)
{
    int rc;
    lsp_socket_t child = NULL;

    if(sock->type != CONN_SERVER)
    {
        lsp_err(tag, "lsp_accept sock type is not CONN_SERVER\n");
        return NULL;
    }

    if(sock->state != CONN_LISTEN)
    {
        lsp_err(tag, "lsp_accept sock is not in listen state\n");
        return NULL;
    }

    // wait happens on queue
    rc = lsp_queue_pop(sock->children, &child, timeout);
    if(rc != LSP_ERR_NONE && rc != LSP_ERR_TIMEOUT)
    {
        lsp_err(tag, "lsp_accept error on rxq %d\n", rc);
    }

    child->type = CONN_CHILD;
    child->parent = sock;

    return child;
}

int lsp_connect(lsp_socket_t sock, lsp_sockaddr_t *sockaddr, size_t addrlen)
{
    if(sockaddr->port == LSP_PORT_ANY) 
    {
        sock->attr.rport = LSP_PACKET_PORT_MAX + 1;
    }
    else if(sockaddr->port > LSP_PACKET_PORT_MAX)
    {
        lsp_err(tag, "lsp_connect invalid port %u, portrange: 0-%u + (LSP_PORT_ANY for default)\n",
            sockaddr->port, LSP_PACKET_PORT_MAX);
        return LSP_ERR_PORT_INVALID;
    }

    sock->attr.raddr = sockaddr->addr;
    // TODO: add checks of remote address from routing table

    return LSP_ERR_NONE;
}