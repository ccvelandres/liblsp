#ifndef LSP_QUEUE_H
#define LSP_QUEUE_H

#include <stddef.h>

#include "lsp_types.h"

/**
 * @defgroup LSP_QUEUE_FLAGS
 * @brief 
 * @{
 */
#define LSP_QUEUE_FLAG_ZERO_COPY (1 << 0)
/**@}*/

typedef void * lsp_queue_handle_t;

/**
 * @brief create a queue
 * 
 * @param len max len of queue
 * @param flags flags for the queue. see @ref LSP_QUEUE_FLAGS
 * @return lsp_queue_handle_t* 
 */
lsp_queue_handle_t *lsp_queue_create(int len, int flags);
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
 * @return int #LSP_SUCCESS on success, otherwise an error code
 */
int lsp_queue_enqueue(lsp_queue_handle_t *handle, const void *const data, const size_t len, const int timeout);

/**
 * @brief pushes data to queue (zero-copy variant)
 * 
 * @param handle pointer to queue handle
 * @param data pointer to data (must be allocated with lsp_*alloc)
 * @param len length of data
 * @param timeout timeout in ms
 * @return int #LSP_SUCCESS on success, otherwise an error code
 */
int lsp_queue_enqueue_zc(lsp_queue_handle_t *handle, const void *const data, const size_t len, const int timeout);

/**
 * @brief pops data from queue
 * 
 * @param handle pointer to queue handle
 * @param data pointer to pointer
 * @param len length of data
 * @param timeout timeout in ms
 * @return int #LSP_SUCCESS on success, otherwise an error code
 */
int lsp_queue_dequeue(lsp_queue_handle_t *handle, void *data, size_t *len, const int timeout);

/**
 * @brief pops data from queue (zero-copy variant)
 * 
 * @param handle pointer to queue handle
 * @param data pointer to pointer
 * @param len length of data
 * @param timeout timeout in ms
 * @return int #LSP_SUCCESS on success, otherwise an error code
 */
int lsp_queue_dequeue_zc(lsp_queue_handle_t *handle, void **data, size_t *len, const int timeout);


int lsp_queue_len(lsp_queue_handle_t *handle);
int lsp_queue_peeksize(lsp_queue_handle_t *handle);
#endif