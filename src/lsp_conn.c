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
#include "lsp_conn.h"
#include "lsp_memory.h"
#include "lsp_buffer.h"
#include "lsp_log.h"

#include "string.h"

#ifndef LSP_CONN_EGROUP_POOL
#define LSP_CONN_EGROUP_POOL 0
#endif

#ifndef LSP_CONN_PERSISTENT_RXQUEUE
#define LSP_CONN_PERSISTENT_RXQUEUE 0
#endif

static const char *tag = "lsp_conn";

/** LSP Connection Pool */
static lsp_conn_t *conn_pool;

#if (LSP_CONN_EGROUP_POOL) 
/** LSP Connection Event Group Pool */
static struct lsp_egroup_handle_s *egroup_pool;
#endif

/** LSP Connection Mutex */
static lsp_mutex_t conn_mutex;

static const lsp_connattr_t def_conn_attr = {
    .priority = LSP_CONN_PRIO_DEF,
    .lport = LSP_PORT_ANY,
    .laddr = LSP_ADDR_ANY,
    .rport = LSP_PORT_ANY,
    .raddr = LSP_ADDR_ANY,
    .flags = 0};

int lsp_conn_init()
{
    int rc;
    LSP_ASSERT(conn_pool == NULL, "%s: is called twice\n", __FUNCTION__);
    size_t blocksize = lsp_conf->conn_max * sizeof(lsp_conn_t);

    lsp_mutex_init(&conn_mutex);

    conn_pool = lsp_malloc(blocksize);
    if (conn_pool == NULL)
    {
        lsp_verb(tag, "%s: could not allocate conn_pool\n", __FUNCTION__);
        return -LSP_ERR_NOMEM;
    }
    memset(conn_pool, 0, blocksize);
    lsp_verb(tag, "%s: allocated %d bytes for conn_pool poolsize: %d connsize: %d\n", __FUNCTION__, blocksize, lsp_conf->conn_max, sizeof(lsp_conn_t));
    
#if (LSP_CONN_EGROUP_POOL)
    // we allocate a large block instead of multiple small blocks
    // not actually sure if this would be better than allocating only when needed
    blocksize = sizeof(struct lsp_egroup_handle_s) * lsp_conf->conn_max;
    egroup_pool = lsp_malloc(blocksize);
    if(egroup_pool == NULL)
    {
        lsp_verb(tag, "%s: could not allocate conn_pool\n", __FUNCTION__);
        rc = -LSP_ERR_NOMEM;
        goto conn_err;
    }

    memset(egroup_pool, 0, blocksize);
    lsp_verb(tag, "%s: allocated %d bytes for egroup_pool poolsize: %d egroup_size: %d\n", __FUNCTION__, blocksize, lsp_conf->conn_max, sizeof(struct lsp_egroup_handle_s));

    for(int i = 0; i < lsp_conf->conn_max; ++i)
    {
        rc = lsp_egroup_init(&egroup_pool[i]);
        if(rc != LSP_ERR_NONE)
            goto egroup_err;
        conn_pool[i].egroup = &egroup_pool[i];
    }

egroup_err:
    lsp_free(egroup_pool);
#endif
conn_err:
    lsp_free(conn_pool);
end:
    return rc;
}

lsp_conn_t *lsp_conn_alloc()
{
    int rc;
    lsp_conn_t *conn = NULL;

    lsp_mutex_lock(&conn_mutex, LSP_TIMEOUT_MAX);

    // look for free connection
    for (int i = 0; i < LSP_DEFAULT_MAX_CONNECTIONS; ++i)
        if (conn_pool[i].state == CONN_FREE)
        {
            conn = &conn_pool[i];
            break;
        }

    lsp_mutex_unlock(&conn_mutex);

    if (conn == NULL)
    {
        lsp_verb(tag, "%s: max connections reached\n", __FUNCTION__);
        return NULL;
    }

    // create queue if not exist
    if (conn->rx_queue == NULL)
    {
        conn->rx_queue = lsp_queue_create(lsp_conf->conn_queuelen, sizeof(lsp_buffer_t *));
        if (conn->rx_queue == NULL)
        {
            lsp_err(tag, "%s: could not create rx queue for lsp_conn\n", __FUNCTION__);
            return NULL;
        }
    }

#if !(LSP_CONN_EGROUP_POOL) 
    // create egroup if 1st time
    if (conn->egroup == NULL)
    {
        conn->egroup = lsp_egroup_create();
        if (conn->egroup == NULL)
        {
            lsp_err(tag, "%s: could not create event group for lsp_conn\n", __FUNCTION__);
            return NULL;
        }
    }
#endif

    conn->state = CONN_CLOSED;
    conn->parent = NULL;
    conn->attr = def_conn_attr;
    conn->rcv_timeout = LSP_TIMEOUT_MAX;
    conn->snd_timeout = LSP_TIMEOUT_MAX;
    conn->s_opt = 0;

    return conn;
err:
    return NULL;
}

int lsp_conn_close(lsp_conn_t *conn)
{
    int rc;
    lsp_buffer_t *buff;
    lsp_conn_t *child;

    if (conn->state <= CONN_CLOSED)
    {
        lsp_verb(tag, "%s: connection already closed\n", __FUNCTION__);
        return LSP_ERR_NONE;
    }

    // Set connection to closed
    conn->state = CONN_CLOSED;
    lsp_list_del(&conn->portlist);

    // flush rxq
    rc = lsp_conn_rxq_flush(conn);
    if (rc != LSP_ERR_NONE)
        goto end;

#if (LSP_CONN_PERSISTENT_RXQUEUE)
    rc = lsp_queue_destroy(conn->rx_queue);
    if(rc == LSP_ERR_NONE)
        conn->rx_queue = NULL;
    else 
        lsp_err(tag, "%s: could not destroy rx_queue for lsp_conn\n", __FUNCTION__);
#endif

#if !(LSP_CONN_EGROUP_POOL)
    rc = lsp_egroup_destroy(conn->egroup);
    if(rc == LSP_ERR_NONE)
        conn->egroup = NULL;
    else 
        lsp_err(tag, "%s: could not destroy egroup for lsp_conn\n", __FUNCTION__);
#endif

end:
    return rc;
}

int lsp_conn_free(lsp_conn_t *conn)
{
    int rc;
    lsp_socket_t sock;
    if (conn->state != CONN_CLOSED)
        rc = lsp_conn_close(conn);
    if (rc != LSP_ERR_NONE)
        goto end;

    if (conn->type == CONN_SERVER)
    {
        while (lsp_queue_pop(conn->children, &sock, 0) != LSP_ERR_QUEUE_EMPTY)
        {
            lsp_conn_free(sock);
        }
        lsp_queue_destroy(conn->children);
    }

    // free the connection
    conn->state = CONN_FREE;
end:
    return rc;
}

int lsp_conn_rxq_flush(lsp_conn_t *conn)
{
    lsp_buffer_t *b;
    if (conn->rx_queue == NULL)
        return LSP_ERR_INVALID;
    while (lsp_queue_pop(conn->rx_queue, &b, 0) != LSP_ERR_NONE)
    {
        lsp_buffer_free(b);
    }
    return LSP_ERR_NONE;
}

int lsp_conn_rxq_push(lsp_conn_t *conn, lsp_buffer_t *buffer)
{
    return lsp_queue_push(conn->rx_queue, &buffer, LSP_DEFAULT_QUEUE_TIMEOUT_MS);
}