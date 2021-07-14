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

#include "lsp_egroup.h"
#include "lsp_log.h"
#include "lsp_mutex.h"
#include "lsp_memory.h"
#include "lsp_time.h"

#include <time.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>

#ifndef LSP_POSIX
#error "This source is for POSIX systems only"
#endif

/** TODO: i feel so bad with this implementation, probably needs refactoring in the future */

static const char *tag = "lsp_egroup";

int lsp_egroup_init(lsp_egroup_handle_t handle)
{
    int rc;
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);

    if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) != 0)
        goto init_err;

    if (pthread_cond_init(&handle->cond, &attr) != 0)
        goto init_err;

    handle->event_bits = 0;
    pthread_mutex_init(&handle->mutex, NULL);
init_err:
    pthread_condattr_destroy(&attr);
end:
    return rc;
}

lsp_egroup_handle_t lsp_egroup_create()
{
    int rc;
    lsp_egroup_handle_t handle = NULL;
    pthread_condattr_t attr;
    size_t blocksize = sizeof(struct lsp_egroup_handle_s);

    handle = lsp_malloc(blocksize);
    if (handle == NULL)
        goto end;

    memset(handle, 0, blocksize);
    pthread_condattr_init(&attr);

    if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) != 0)
        goto init_err;

    if (pthread_cond_init(&handle->cond, &attr) != 0)
        goto init_err;

    pthread_condattr_destroy(&attr);
    pthread_mutex_init(&handle->mutex, NULL);
    return handle;
init_err:
    pthread_condattr_destroy(&attr);
alloc_err:
    lsp_free(handle);
end:
    return NULL;
}

void lsp_egroup_destroy(lsp_egroup_handle_t handle)
{
    /** TODO: add mechanism check and wake waiting threads before destroying pthread vars */
    pthread_cond_destroy(&handle->cond);
    pthread_mutex_destroy(&handle->mutex);
    lsp_free(handle);
}

static inline int egroup_wait_internal(pthread_cond_t *cond, pthread_mutex_t *mutex, uint32_t timeout)
{
    struct timespec ts;
    int rc;

    if (timeout != LSP_TIMEOUT_MAX)
    {
        rc = clock_gettime(CLOCK_MONOTONIC, &ts);
        if (rc < 0)
        {
            lsp_verb(tag, "%s: failed to get time\n", __FUNCTION__);
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

lsp_egroup_bits_t lsp_egroup_wait(lsp_egroup_handle_t handle, lsp_egroup_bits_t bits, int clearOnExit, int waitAll, uint32_t timeout)
{
    lsp_egroup_bits_t ebits;
    uint32_t currrent_time, max_time = lsp_gettime_ms() + timeout;
    uint32_t remaining_timeout = timeout;
    int rc;

    rc = lsp_mutex_lock(&handle->mutex, remaining_timeout);
    if (rc != LSP_ERR_NONE)
        goto err;

    if (waitAll)
    {
        if (timeout == LSP_TIMEOUT_MAX)
        {
            while ((handle->event_bits & bits) != bits)
            {
                
                rc = egroup_wait_internal(&handle->cond, &handle->mutex, timeout);
                if (rc != 0)
                {
                    lsp_verb(tag, "%s: error %d waiting for event\n", __FUNCTION__, rc);
                    goto mutex_err;
                }
            }
        }
        else if (timeout > 0)
        {
            while (remaining_timeout > 0)
            {
                currrent_time = lsp_gettime_ms();
                remaining_timeout = (max_time > currrent_time ? max_time - currrent_time : 0);
                
                rc = egroup_wait_internal(&handle->cond, &handle->mutex, remaining_timeout);
                if (rc != 0 && rc != ETIMEDOUT)
                {
                    lsp_verb(tag, "%s: error %d waiting for event\n", __FUNCTION__, rc);
                    goto mutex_err;
                }
                else if ((handle->event_bits & bits) == bits)
                    goto mutex_err;
            }
        }
        else
        {
            // no timeout, so just return
            goto mutex_err;
        }
    }
    else
    {
        if (timeout == LSP_TIMEOUT_MAX)
        {
            while (!(handle->event_bits & bits))
            {
                rc = egroup_wait_internal(&handle->cond, &handle->mutex, timeout);
                if (rc != 0)
                {
                    lsp_verb(tag, "%s: error %d waiting for event\n", __FUNCTION__, rc);
                    goto mutex_err;
                }
            }
        }
        else if (timeout > 0)
        {
            while (remaining_timeout > 0)
            {
                currrent_time = lsp_gettime_ms();
                remaining_timeout = (max_time > currrent_time ? max_time - currrent_time : 0);
                
                rc = egroup_wait_internal(&handle->cond, &handle->mutex, remaining_timeout);
                if (rc != 0 && rc != ETIMEDOUT)
                {
                    lsp_verb(tag, "%s: error %d waiting for event\n", __FUNCTION__, rc);
                    goto mutex_err;
                }
                else if ((handle->event_bits & bits))
                    goto mutex_err;
            }
        }
        else
        {
            // no timeout, so just return
            goto mutex_err;
        }
    }

mutex_err:
    ebits = handle->event_bits;
    if(clearOnExit){
        handle->event_bits = 0;
    }
    else {
        handle->event_bits &= ~bits;
    }

    lsp_mutex_unlock(&handle->mutex);
err:
    return ebits;
}

lsp_egroup_bits_t lsp_egroup_set(lsp_egroup_handle_t handle, lsp_egroup_bits_t bits)
{
    int rc;

    rc = lsp_mutex_lock(&handle->mutex, LSP_DEFAULT_MUTEX_TIMEOUT_MS);
    if (rc != LSP_ERR_NONE)
    {
        lsp_err(tag, "%s: error locking mutex, this shouldn't happen\n", __FUNCTION__);
        goto err;
    }

    handle->event_bits |= bits;
    pthread_cond_broadcast(&handle->cond);
mutex_err:
    lsp_mutex_unlock(&handle->mutex);
err:
    return handle->event_bits;
}
