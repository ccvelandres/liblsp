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

#include "lsp_mutex.h"
#include "lsp_log.h"

#include <time.h>
#include <string.h>

static const char *tag = "lsp_mutex";

lsp_mutex_t *lsp_mutex_create()
{
    return NULL;
}
int lsp_mutex_init(lsp_mutex_t *mutex)
{
    int rc = pthread_mutex_init(mutex, NULL);
    if (rc)
    {
        lsp_verb(tag, "could not initialize mutex %d:%s", rc, strerror(rc));
        return -LSP_ERR_MUTEX;
    }
    else
        return LSP_ERR_NONE;
}
int lsp_mutex_destroy(lsp_mutex_t *mutex)
{
    int rc = pthread_mutex_destroy(mutex);
    if (rc)
    {
        lsp_verb(tag, "could not destroy mutex %d:%s\n", rc, strerror(rc));
        return -LSP_ERR_MUTEX;
    }
    else
        return LSP_ERR_NONE;
}

int lsp_mutex_lock(lsp_mutex_t *mutex, int timeout)
{
    int rc;
    struct timespec abs_time;
    memset(&abs_time, 0, sizeof(struct timespec));
    abs_time.tv_nsec = timeout * 1000;
    rc = pthread_mutex_timedlock(mutex, &abs_time);
    if (rc)
    {
        lsp_verb(tag, "could not lock mutex %d:%s\n", rc, strerror(rc));
        return -LSP_ERR_MUTEX;
    }
    else
        return LSP_ERR_NONE;
}

int lsp_mutex_unlock(lsp_mutex_t *mutex)
{
    int rc = pthread_mutex_unlock(mutex);
    if (rc)
    {
        lsp_verb(tag, "could not unlock mutex %d:%s\n", rc, strerror(rc));
        return -LSP_ERR_MUTEX;
    }
    else
        return LSP_ERR_NONE;
}