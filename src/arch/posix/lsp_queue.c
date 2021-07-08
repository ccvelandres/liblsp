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

#include "lsp_queue.h"
#include "lsp_memory.h"
#include "lsp_mutex.h"
#include "lsp_log.h"

#include "signal.h"
#include "string.h"
#include "errno.h"

#ifdef LSP_QUEUE_ALIGNMEM
#define ENTRY_FIND(ptr, index, itemsize) \
    ((void *)(ptr + (index * ALIGNED_SIZE(itemsize))))
#else
#define ENTRY_FIND(ptr, index, itemsize) \
    ((void *)(ptr + (index * itemsize)))
#endif

static const char *tag = "posix/lsp_queue";

/** struct and buffer is implemented as contiguous chunk */
typedef struct _lsp_queue_handle
{
    sig_atomic_t length, head, tail, waiting_full, waiting_empty;
    size_t queue_size, itemsize;
    lsp_mutex_t mutex;
    pthread_cond_t cond_full, cond_empty;
    uint8_t *data;
} _lsp_queue_handle_t;

static inline int queue_wait(pthread_cond_t *cond, pthread_mutex_t *mutex, int timeout)
{
    struct timespec ts;
    int rc;

    if (timeout > 0)
    {
        rc = clock_gettime(CLOCK_MONOTONIC, &ts);
        if (rc < 0)
        {
            lsp_verb(tag, "failed to get time\n");
            goto notimeout;
        }
        else
            goto timeout;
    }

notimeout:
    rc = pthread_cond_wait(cond, mutex);
    goto end;
timeout:
    ts.tv_sec += timeout / 1000;
    ts.tv_nsec += (timeout % 1000) * 1000000;
    rc = pthread_cond_timedwait(cond, mutex, &ts);
end:

    return rc;
}

static inline int queue_init_pthread_cond(pthread_cond_t *cond)
{
    int rc;
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    rc = pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    if (rc == 0)
        rc = pthread_cond_init(cond, &attr);
    pthread_condattr_destroy(&attr);
    return rc;
}

lsp_queue_handle_t lsp_queue_create(int length, size_t itemsize)
{
    _lsp_queue_handle_t *hdl;
    size_t blocksize;
#ifdef LSP_QUEUE_ALIGNMEM
    blocksize = ALIGNED_SIZEOF(_lsp_queue_handle_t) + (length * ALIGNED_SIZE(itemsize));
#else
    blocksize = ALIGNED_SIZEOF(_lsp_queue_handle_t) + (length * itemsize);
    if (!IS_ALIGNED(itemsize))
        lsp_warn(tag, "queue_create itemsize is unaligned and mem alignment is disabled\n");
#endif
    hdl = lsp_malloc(blocksize);
    if (hdl == NULL)
        goto err;

    memset(hdl, 0, blocksize);
    if (lsp_mutex_init(&hdl->mutex) != LSP_ERR_NONE ||
        queue_init_pthread_cond(&hdl->cond_full) != 0 ||
        queue_init_pthread_cond(&hdl->cond_empty) != 0)
    {
        lsp_verb(tag, "queue_create could init pthread elements\n");
        goto alloc_err;
    }

    hdl->head = 0;
    hdl->tail = 0;
    hdl->length = 0;
    hdl->queue_size = length;
    hdl->itemsize = itemsize;
    hdl->data = ((unsigned char *)hdl + ALIGNED_SIZEOF(_lsp_queue_handle_t));

    lsp_verb(tag, "queue_create block @ %p blocksize: %d controlblock: %d data @ %p len: %d itemsize: %d (%d)\n",
             hdl,
             blocksize,
             ALIGNED_SIZEOF(_lsp_queue_handle_t),
             hdl->data,
             length,
             itemsize,
#ifdef LSP_QUEUE_ALIGNMEM
             ALIGNED_SIZE(itemsize)
#else
             itemsize);
#endif
    return (lsp_queue_handle_t)hdl;
alloc_err:
    lsp_free(hdl);
err:
    return NULL;
}

int lsp_queue_destroy(lsp_queue_handle_t handle)
{
    _lsp_queue_handle_t *hdl = (_lsp_queue_handle_t *)handle;
    if(hdl->waiting_empty || hdl->waiting_full)
        return LSP_ERR_RESOURCE_IN_USE;
    pthread_cond_destroy(&hdl->cond_empty);
    pthread_cond_destroy(&hdl->cond_full);
    lsp_mutex_destroy(&hdl->mutex);
    lsp_free(hdl);
}

int lsp_queue_push(lsp_queue_handle_t handle, const void *const data, const int timeout)
{
    int rc, idx;
    uint8_t *entryPtr;
    _lsp_queue_handle_t *hdl = (_lsp_queue_handle_t *)handle;

    // start of protected access
    rc = lsp_mutex_lock(&hdl->mutex, timeout);
    if (rc != LSP_ERR_NONE)
        goto err;

    if (hdl->length >= hdl->queue_size)
    {
        hdl->waiting_full++;
        rc = queue_wait(&hdl->cond_full, &hdl->mutex, timeout);
        hdl->waiting_full--;
        if (rc)
        {
            rc = rc == ETIMEDOUT ? LSP_ERR_TIMEOUT : LSP_ERR_INVALID;
            goto mutex_err;
        }
    }

    entryPtr = ENTRY_FIND(hdl->data, hdl->tail, hdl->itemsize);
    memcpy(entryPtr, data, hdl->itemsize);
    lsp_verb(tag, "queue_push idx: %d @ %p\n", hdl->tail, entryPtr);

    hdl->tail = ++hdl->tail % hdl->queue_size;
    hdl->length++;
    pthread_cond_signal(&hdl->cond_empty);
    rc = LSP_ERR_NONE;
mutex_err:
    lsp_mutex_unlock(&hdl->mutex);
err:
    return rc;
}

int lsp_queue_pop(lsp_queue_handle_t handle, void *data, const int timeout)
{
    int rc;
    void *entryPtr;
    _lsp_queue_handle_t *hdl = (_lsp_queue_handle_t *)handle;

    // start of protected access
    rc = lsp_mutex_lock(&hdl->mutex, timeout);
    if (rc != LSP_ERR_NONE)
        goto err;

    if (hdl->length <= 0)
    {
        hdl->waiting_empty++;
        rc = queue_wait(&hdl->cond_empty, &hdl->mutex, timeout);
        hdl->waiting_empty--;
        if (rc)
        {
            rc = rc == ETIMEDOUT ? LSP_ERR_TIMEOUT : LSP_ERR_INVALID;
            goto mutex_err;
        }
    }

    entryPtr = ENTRY_FIND(hdl->data, hdl->head, hdl->itemsize);
    memcpy(data, entryPtr, hdl->itemsize);
    lsp_verb(tag, "queue_pop  idx: %d @ %p\n", hdl->head, entryPtr);

    hdl->head = ++hdl->head % hdl->queue_size;
    hdl->length--;
    rc = LSP_ERR_NONE;
mutex_err:
    lsp_mutex_unlock(&hdl->mutex);
err:
    return rc;
}

int lsp_queue_length(lsp_queue_handle_t handle)
{
    _lsp_queue_handle_t *hdl = (_lsp_queue_handle_t *)handle;
    return hdl->queue_size;
}

int lsp_queue_itemsize(lsp_queue_handle_t handle)
{
    _lsp_queue_handle_t *hdl = (_lsp_queue_handle_t *)handle;
    return hdl->itemsize;
}

int lsp_queue_clear(lsp_queue_handle_t handle)
{
    _lsp_queue_handle_t *hdl = (_lsp_queue_handle_t *)handle;
    if(hdl->waiting_empty || hdl->waiting_full)
        return LSP_ERR_RESOURCE_IN_USE;
    lsp_mutex_lock(&hdl->mutex, -1);
    hdl->head = 0;
    hdl->tail = 0;
    hdl->length = 0;
    lsp_mutex_unlock(&hdl->mutex);
    return LSP_ERR_NONE;
}