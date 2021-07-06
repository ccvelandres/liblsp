#ifndef LSP_QUEUE_H
#define LSP_QUEUE_H

#include <stddef.h>
#include "signal.h"

#include "lsp_types.h"
#include "lsp_mutex.h"

/** LSP Queue entry struct */
typedef struct lsp_queue_entry_s
{
    size_t item_size;
    void *data;
} lsp_queue_entry_t;

/** LSP Queue Handle */
typedef struct lsp_queue_handle_s
{
    lsp_queue_entry_t *arr;
    sig_atomic_t length, head, tail;
    size_t queue_size;
    lsp_mutex_t mutex;
} lsp_queue_handle_t;

/**
 * @brief create a queue
 * 
 * @param len max len of queue
 * @return lsp_queue_handle_t* 
 */
lsp_queue_handle_t *lsp_queue_create(int len);
/**
 * @brief destroys the queue
 * 
 * @param handle pointer to queue handle
 */
void lsp_queue_destroy(lsp_queue_handle_t *handle);

/**
 * @brief pushes data to queue
 * 
 * @param handle pointer to queue handle
 * @param data pointer to data
 * @param len length of data
 * @param timeout timeout in ms
 * @return int #LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_queue_enqueue(lsp_queue_handle_t *handle, const void *const data, const size_t len, const int timeout);

/**
 * @brief pushes data to queue (zero-copy variant, data must be allocated with lsp_*alloc)
 * 
 * @param handle pointer to queue handle
 * @param data pointer to data (must be allocated with lsp_*alloc)
 * @param len length of data
 * @param timeout timeout in ms
 * @return int #LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_queue_enqueue_zc(lsp_queue_handle_t *handle, const void *const data, const size_t len, const int timeout);

/**
 * @brief pops data from queue
 * 
 * @param handle pointer to queue handle
 * @param data pointer to pointer
 * @param len length of data
 * @param timeout timeout in ms
 * @return int #LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_queue_dequeue(lsp_queue_handle_t *handle, void *data, size_t *len, const int timeout);

/**
 * @brief pops data from queue (zero-copy variant, data must be freed with lsp_free)
 * 
 * @param handle pointer to queue handle
 * @param data pointer to pointer
 * @param len length of data
 * @param timeout timeout in ms
 * @return int #LSP_ERR_NONE on success, otherwise an error code
 */
int lsp_queue_dequeue_zc(lsp_queue_handle_t *handle, void **data, size_t *len, const int timeout);


int lsp_queue_len(lsp_queue_handle_t *handle);
int lsp_queue_peeksize(lsp_queue_handle_t *handle);
#endif