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

#ifndef LSP_DEFAULTS_H
#define LSP_DEFAULTS_H

#include "lsp_defaults_user.h"

#ifndef LSP_DEFAULT_ADDR
#define LSP_DEFAULT_ADDR 0xAE55
#endif

#ifndef LSP_DEFAULT_HOSTNAME
#define LSP_DEFAULT_HOSTNAME "LSP Hostname"
#endif

#ifndef LSP_DEFAULT_MACHINENAME
#define LSP_DEFAULT_MACHINENAME "LSP Machinename"
#endif

#ifndef LSP_DEFAULT_LSPREV
#define LSP_DEFAULT_LSPREV "LSP Revision"
#endif

#ifndef LSP_DEFAULT_BUFFER_HEADER_LEN
#define LSP_DEFAULT_BUFFER_HEADER_LEN 8
#endif

#ifndef LSP_DEFAULT_MAX_CONNECTIONS
#define LSP_DEFAULT_MAX_CONNECTIONS 32
#endif

#ifndef LSP_DEFAULT_CONN_QUEUELEN
#define LSP_DEFAULT_CONN_QUEUELEN 4
#endif

#ifndef LSP_DEFAULT_QUEUE_TIMEOUT_MS
#define LSP_DEFAULT_QUEUE_TIMEOUT_MS 100
#endif

#ifndef LSP_DEFAULT_MUTEX_TIMEOUT_MS
#define LSP_DEFAULT_MUTEX_TIMEOUT_MS 100
#endif

#ifndef LSP_DEFAULT_ROUTE_EXPIRY_MS
#define LSP_DEFAULT_ROUTE_EXPIRY_MS 500
#endif

#ifndef LSP_DEFAULT_CORE_STACK_SIZE
#define LSP_DEFAULT_CORE_STACK_SIZE 2048
#endif

#ifndef LSP_DEFAULT_CORE_PRIORITY
#define LSP_DEFAULT_CORE_PRIORITY 7
#endif

#ifndef LSP_DEFAULT_CORE_STACK_SIZE
#define LSP_DEFAULT_CORE_STACK_SIZE 2048
#endif

#ifndef LSP_DEFAULT_CORE_EVQUEUE_LEN
#define LSP_DEFAULT_CORE_EVQUEUE_LEN 32
#endif

#endif