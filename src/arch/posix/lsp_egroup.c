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

#if (!LSP_POSIX)
#error "This source is for POSIX systems only"
#endif

/** TODO: i feel so bad with this implementation, probably needs refactoring in the future */

static const char *tag = "lsp_egroup";

typedef struct
{
    uint32_t event_bits;
    pthread_cond_t cond;
    lsp_mutex_t mutex;
} lsp_egroup_posix_t;

lsp_egroup_handle_t lsp_egroup_create()
{
    int rc;
    lsp_egroup_posix_t *hdl = NULL;
    pthread_condattr_t attr;
    size_t blocksize = sizeof(lsp_egroup_posix_t);

    hdl = lsp_malloc(blocksize);
    if (hdl == NULL)
        goto end;

    memset(hdl, 0, blocksize);
    pthread_condattr_init(&attr);

    if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) != 0)
        goto init_err;

    if (pthread_cond_init(&hdl->cond, &attr) != 0)
        goto init_err;

    pthread_condattr_destroy(&attr);
    return (lsp_egroup_handle_t)hdl;
init_err:
    pthread_condattr_destroy(&attr);
alloc_err:
    lsp_free(hdl);
end:
    return NULL;
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
    /** TODO: add computation for remaining time, as of now, timeout is reused */
    lsp_egroup_posix_t *hdl = (lsp_egroup_posix_t *)handle;
    lsp_egroup_bits_t ebits;
    uint32_t currrent_time, max_time = lsp_gettime_ms() + timeout;
    uint32_t remaining_timeout = timeout;
    int rc;

    rc = lsp_mutex_lock(&hdl->mutex, remaining_timeout);
    if (rc != LSP_ERR_NONE)
        goto err;

    if (waitAll)
    {
        if (timeout == LSP_TIMEOUT_MAX)
        {
            while ((hdl->event_bits & bits) != bits)
            {
                
                rc = egroup_wait_internal(&hdl->cond, &hdl->mutex, timeout);
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
                
                rc = egroup_wait_internal(&hdl->cond, &hdl->mutex, remaining_timeout);
                if (rc != 0 && rc != ETIMEDOUT)
                {
                    lsp_verb(tag, "%s: error %d waiting for event\n", __FUNCTION__, rc);
                    goto mutex_err;
                }
                else if ((hdl->event_bits & bits) == bits)
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
            while (!(hdl->event_bits & bits))
            {
                rc = egroup_wait_internal(&hdl->cond, &hdl->mutex, timeout);
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
                
                rc = egroup_wait_internal(&hdl->cond, &hdl->mutex, remaining_timeout);
                if (rc != 0 && rc != ETIMEDOUT)
                {
                    lsp_verb(tag, "%s: error %d waiting for event\n", __FUNCTION__, rc);
                    goto mutex_err;
                }
                else if ((hdl->event_bits & bits))
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
    ebits = hdl->event_bits;
    if(clearOnExit){
        hdl->event_bits = 0;
    }
    else {
        hdl->event_bits &= ~bits;
    }

    lsp_mutex_unlock(&hdl->mutex);
err:
    return ebits;
}

lsp_egroup_bits_t lsp_egroup_set(lsp_egroup_handle_t handle, lsp_egroup_bits_t bits)
{
    int rc;
    lsp_egroup_posix_t *hdl = (lsp_egroup_posix_t *)handle;

    rc = lsp_mutex_lock(&hdl->mutex, LSP_DEFAULT_MUTEX_TIMEOUT_MS);
    if (rc != LSP_ERR_NONE)
    {
        lsp_err(tag, "%s: error locking mutex, this shouldn't happen\n", __FUNCTION__);
        goto err;
    }

    hdl->event_bits |= bits;
    pthread_cond_broadcast(&hdl->cond);
mutex_err:
    lsp_mutex_unlock(&hdl->mutex);
err:
    return hdl->event_bits;
}
