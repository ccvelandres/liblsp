#include "lsp_queue.h"
#include "lsp_memory.h"
#include "lsp_log.h"

#include "string.h"

lsp_queue_handle_t *lsp_queue_create(int length)
{
    lsp_queue_handle_t *hdl = lsp_calloc(1, sizeof(lsp_queue_handle_t));
    if (hdl == NULL)
        goto err;

    hdl->arr = lsp_calloc(length, sizeof(lsp_queue_entry_t));
    if (hdl->arr == NULL)
        goto alloc_err;
    hdl->head = 0;
    hdl->tail = 0;
    hdl->length = 0;
    hdl->queue_size = length;
    if (lsp_mutex_init(&hdl->mutex) != LSP_ERR_NONE)
        goto alloc_err;

    return (void *)hdl;
alloc_err:
    lsp_free(hdl);
err:
    return NULL;
}

void lsp_queue_destroy(lsp_queue_handle_t *handle)
{
    lsp_queue_handle_t *hdl = (lsp_queue_handle_t *)handle;
    lsp_queue_entry_t *entry;
    lsp_mutex_lock(&hdl->mutex, -1);

    // walk through and free entries
    for (int i; i < hdl->queue_size; ++i)
    {
        void *data = hdl->arr[hdl->tail].data;
        if (data)
            lsp_free(data);
    }

    lsp_mutex_unlock(&hdl->mutex);
    lsp_free(handle);
}

int lsp_queue_enqueue(lsp_queue_handle_t *handle, const void *const data, const size_t len, const int timeout)
{
    int rc, idx;
    lsp_queue_entry_t *entry;
    lsp_queue_handle_t *hdl = (lsp_queue_handle_t *)handle;

    // rely on atomic size
    if (hdl->length >= hdl->queue_size)
        return LSP_ERR_QUEUE_FULL;

    // start of protected access
    rc = lsp_mutex_lock(&hdl->mutex, timeout);
    if (rc != LSP_ERR_NONE)
        goto err;

    entry = &hdl->arr[hdl->tail];

    rc = -LSP_ERR_NOMEM;
    entry->data = lsp_malloc(len);
    if (entry->data == NULL)
        goto err;
    entry->item_size = len;
    memcpy(entry->data, data, len);

    hdl->tail = ++hdl->tail % hdl->queue_size;
    hdl->length++;
    rc = LSP_ERR_NONE;
mutex_err:
    lsp_mutex_unlock(&hdl->mutex);
err:
    return rc;
}

int lsp_queue_enqueue_zc(lsp_queue_handle_t *handle, const void *const data, const size_t len, const int timeout)
{
    int rc, idx;
    lsp_queue_entry_t *entry;
    lsp_queue_handle_t *hdl = (lsp_queue_handle_t *)handle;

    // rely on atomic size
    if (hdl->length >= hdl->queue_size)
        return LSP_ERR_QUEUE_FULL;

    // start of protected access
    rc = lsp_mutex_lock(&hdl->mutex, timeout);
    if (rc != LSP_ERR_NONE)
        goto err;

    entry = &hdl->arr[hdl->tail];

    entry->data = (void *)data;
    entry->item_size = len;

    hdl->tail = ++hdl->tail % hdl->queue_size;
    hdl->length++;
    rc = LSP_ERR_NONE;
mutex_err:
    lsp_mutex_unlock(&hdl->mutex);
err:
    return rc;
}

int lsp_queue_dequeue(lsp_queue_handle_t *handle, void *data, size_t *len, const int timeout)
{
    int rc;
    lsp_queue_entry_t *entry;
    lsp_queue_handle_t *hdl = (lsp_queue_handle_t *)handle;

    // rely on atomic size
    if (hdl->length == 0)
        return LSP_ERR_QUEUE_EMPTY;

    // start of protected access
    rc = lsp_mutex_lock(&hdl->mutex, timeout);
    if (rc != LSP_ERR_NONE)
        goto err;

    entry = &hdl->arr[hdl->head];
    if (*len < entry->item_size)
    {
        rc = -LSP_ERR_NOMEM;
        goto mutex_err;
    }

    memcpy(data, entry->data, entry->item_size);
    *len = entry->item_size;
    entry->data = NULL;
    entry->item_size = 0;

    hdl->head = ++hdl->head % hdl->queue_size;
    hdl->length--;
    rc = LSP_ERR_NONE;
mutex_err:
    lsp_mutex_unlock(&hdl->mutex);
err:
    return rc;
}

int lsp_queue_dequeue_zc(lsp_queue_handle_t *handle, void **data, size_t *len, const int timeout)
{
    int rc;
    lsp_queue_entry_t *entry;
    lsp_queue_handle_t *hdl = (lsp_queue_handle_t *)handle;

    // rely on atomic size
    if (hdl->length == 0)
        return LSP_ERR_QUEUE_EMPTY;

    // start of protected access
    rc = lsp_mutex_lock(&hdl->mutex, timeout);
    if (rc != LSP_ERR_NONE)
        goto err;

    entry = &hdl->arr[hdl->head];
    *data = entry->data;
    *len = entry->item_size;
    entry->data = NULL;
    entry->item_size = 0;

    hdl->head = ++hdl->head % hdl->queue_size;
    hdl->length--;
    rc = LSP_ERR_NONE;
mutex_err:
    lsp_mutex_unlock(&hdl->mutex);
err:
    return rc;
}

int lsp_queue_length(lsp_queue_handle_t *handle)
{
    lsp_queue_handle_t *hdl = (lsp_queue_handle_t *)handle;
    return hdl->queue_size;
}

int lsp_queue_peeksize(lsp_queue_handle_t *handle)
{
    int rc;
    lsp_queue_handle_t *hdl = (lsp_queue_handle_t *)handle;
    lsp_queue_entry_t *entry = &hdl->arr[hdl->head];
    return entry->item_size;
}