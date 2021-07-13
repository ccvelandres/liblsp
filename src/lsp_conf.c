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
#include "lsp_log.h"

static const char *tag = "lsp_conf";

static lsp_conf_t _lsp_conf = {
    .addr = LSP_DEFAULT_ADDR,
    .hostname = LSP_DEFAULT_HOSTNAME,
    .machinename = LSP_DEFAULT_MACHINENAME,
    .rev = LSP_DEFAULT_LSPREV,
    .conn_max = LSP_DEFAULT_MAX_CONNECTIONS,
    .conn_queuelen = LSP_DEFAULT_CONN_QUEUELEN};

const lsp_conf_t *const lsp_conf = &_lsp_conf;

int lsp_conf_init(lsp_conf_t *conf)
{
    if (conf == NULL)
    {
        lsp_verb(tag, "%s: Default configurations loaded\n", __FUNCTION__);
        return LSP_ERR_NONE;
    }

    if (conf->addr > LSP_PACKET_ADDR_MAX)
    {
        lsp_verb(tag, "%s: address out of range\n", __FUNCTION__);
        return LSP_ERR_ADDR_INVALID;
    }

    if (conf->hostname == NULL)
        lsp_verb(tag, "%s: null hostname, loading defaults\n", __FUNCTION__);

    if (conf->machinename == NULL)
        lsp_verb(tag, "%s: null machinename, loading defaults\n", __FUNCTION__);

    if (conf->rev == NULL)
        lsp_verb(tag, "%s: null rev, loading defaults\n", __FUNCTION__);

    _lsp_conf.addr = conf->addr;
    _lsp_conf.hostname = conf->hostname;
    _lsp_conf.machinename = conf->machinename;
    _lsp_conf.rev = conf->rev;
    _lsp_conf.conn_max = conf->conn_max;
    _lsp_conf.conn_queuelen = conf->conn_queuelen;

    return LSP_ERR_NONE;
}