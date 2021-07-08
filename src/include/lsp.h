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

#ifndef LSP_H
#define LSP_H

#include <stddef.h>

#include "lsp_types.h"

const lsp_conf_t * const lsp_conf;

struct lsp_conf_s
{
    lsp_addr_t addr; /** LSP Address of the system */

    const char *hostname; /** Host name */
    const char *machinename; /** Machine name */
    const char *rev; /** Revision */

    uint8_t conn_max;
    uint8_t conn_queuelen;
};

/**
 * @brief Initialize LSP
 * 
 * @param conf pointer to configuration, leave NULL for defaults
 * @return int LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_init(lsp_conf_t *conf);

#endif