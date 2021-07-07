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

#ifndef LSP_SOCKET_H
#define LSP_SOCKET_H

#include <stddef.h>
#include "lsp_types.h"

/**
 * @brief Creates a new lsp socket
 * 
 * @param domain must be set to AF_LSP
 * @param type LSP_SOCK_STREAM or LSP_SOCK_RAW to send raw packets
 * @param protocol must be LSPPROTO_LSP
 * @return lsp_socket_t 
 */
lsp_socket_t lsp_socket(int domain, int type, int protocol);

/**
 * @brief closes the lsp socket.
 * Named closesocket to prevent name collision with system close and shutdown functions.
 * 
 * @param sock socket
 * @param how not currently used
 */
void lsp_closesocket(lsp_socket_t sock, int how);

/**
 * @brief Binds port to socket
 * 
 * @param sock socket to bind
 * @param sockaddr pointer to sockaddr with details for binding socket
 * @param addrlen not currently used but should be sizeof(lsp_sockaddr_t) for future compatibility
 * @return int LSP_ERR_NONE on success, otherwise an error code 
 */
int lsp_bind(lsp_socket_t sock, lsp_sockaddr_t *sockaddr, size_t addrlen);

/**
 * @brief Set socket to listen for incoming connections
 * 
 * @param sock socket to listen
 * @param backlog max connections for socket 
 * @return int 
 */
int lsp_listen(lsp_socket_t sock, int backlog);

/**
 * @brief Accept incoming connections from socket. 
 * Socket must be placed in listen state
 * 
 * @param sock socket
 * @param timeout timeout in ms
 * @return int LSP_ERR_NONE on success, otherwise LSP_ERR_TIMEOUT
 */
int lsp_accept(lsp_socket_t sock, uint32_t timeout);

/**
 * Connects the socket to remote socket. 
 * This call returns immediately unless protocol requires acknowledgement. (ack not yet supported anyway)
 * 
 * @param sock socket
 * @param addr pointer to sockaddr
 * @param addrlen not currently used but should be sizeof(lsp_sockaddr_t) for future compatibility
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_connect(lsp_socket_t sock, lsp_sockaddr_t *addr, size_t addrlen);

/**
 * @brief Send data on connected socket
 * 
 * @param sock socket
 * @param buf pointer to data
 * @param buflen length of data
 * @param flags not currently used
 * @return int number of bytes sent, otherwise an error code
 */
int lsp_send(lsp_socket_t sock, const void *buf, size_t buflen, uint32_t flags);

/**
 * @brief Send data on socket (without connection)
 * 
 * @param sock socket
 * @param buf pointer to data
 * @param buflen length of data
 * @param flags not currently used
 * @param dstaddr pointer to sockaddr
 * @param addrlen not currently used but should be sizeof(lsp_sockaddr_t) for future compatibility
 * @return int number of bytes sent, otherwise an error code
 */
int lsp_sendto(lsp_socket_t sock, const void *buf, size_t buflen, uint32_t flags, lsp_sockaddr_t *dstaddr, size_t addrlen);

/**
 * @brief Receives data from connected socket.
 * TODO: Add support for zero-copy semantics
 * 
 * @param sock socket
 * @param buf pointer to buffer
 * @param buflen buffer length
 * @param flags not currently used
 * @return int number of bytes written, otherwise an error code
 */
int lsp_recv(lsp_socket_t sock, void *buf, size_t buflen, uint32_t flags);

/**
 * @brief Receives data from specified connection.
 * TODO: Add support for zero-copy semantics
 * 
 * @param sock socket
 * @param buf pointer to buffer
 * @param buflen buffer length
 * @param flags not currently used
 * @param srcaddr pointer to sockaddr
 * @param addrlen not currently used but should be sizeof(lsp_sockaddr_t) for future compatibility
 * @return int number of bytes written, otherwise an error code
 */
int lsp_recvfrom(lsp_socket_t sock, void *buf, size_t buflen, uint32_t flags, lsp_sockaddr_t *srcaddr, size_t addrlen);

/**
 * @brief Set a socket option
 * 
 * @param sock socket
 * @param level not currently used
 * @param opt option being set or modified
 * @param optval pointer to option value
 * @param optlen length of option value
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_setsockopt(lsp_socket_t sock, int level, int opt, const void *optval, size_t optlen);

/**
 * @brief Get a socket option
 * 
 * @param sock socket
 * @param level not currently used
 * @param opt option being retrieved
 * @param optval pointer to buffer to store value
 * @param optlen length of buffer
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_getsockopt(lsp_socket_t sock, int level, int opt, void *optval, size_t optlen);

#endif