#include "lsp_mutex.h"
#include "lsp_log.h"

#include <time.h>
#include <string.h>

lsp_mutex_t *lsp_mutex_create()
{
    return NULL;
}
int lsp_mutex_init(lsp_mutex_t *mutex)
{
    int rc = pthread_mutex_init(mutex, NULL);
    if (rc)
    {
        lsp_verb("could not initialize mutex %d\n", rc);
        return LSP_ERR_MUTEX;
    }
    else
        return LSP_ERR_NONE;
}
int lsp_mutex_destroy(lsp_mutex_t *mutex)
{
    int rc = pthread_mutex_destroy(mutex);
    if (rc)
    {
        lsp_verb("could not destroy mutex %d\n", rc);
        return LSP_ERR_MUTEX;
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
        lsp_verb("could not lock mutex %d\n", rc);
        return LSP_ERR_MUTEX;
    }
    else
        return LSP_ERR_NONE;
}

int lsp_mutex_unlock(lsp_mutex_t *mutex)
{
    int rc = pthread_mutex_unlock(mutex);
    if (rc)
    {
        lsp_verb("could not unlock mutex %d\n", rc);
        return LSP_ERR_MUTEX;
    }
    else
        return LSP_ERR_NONE;
}