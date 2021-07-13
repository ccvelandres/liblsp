#ifndef LSP_THREAD_H
#define LSP_THREAD_H

#include <stddef.h>

#include "lsp_types.h"

#ifdef LSP_POSIX
#include <pthread.h>

/**
 * Platform specific thread handle
 */
typedef pthread_t lsp_thread_handle_t;
/**
 * @brief Platform specific thread return type
 */
typedef void * lsp_thread_return_t;
/**
 * @brief Platform specific thread function
 */
typedef lsp_thread_return_t (*lsp_thread_func_t)(void *arg);

#else 
/**
 * Platform specific thread handle
 */
typedef void lsp_thread_handle_t;
/**
 * @brief Platform specific thread return type
 */
typedef void lsp_thread_return_t;
/**
 * @brief Platform specific thread function
 */
typedef lsp_thread_return_t (*lsp_thread_func_t)(void *arg);
#endif

/**
 * @brief LSP wrapper for creating threads
 * 
 * @param func thread function
 * @param name name of thread
 * @param stack_size stack size of thread
 * @param parameter parameter for thread function
 * @param priority thread priority
 * @param handle reference to created thread
 * @return int LSP_ERR_NONE for success, otherwise an error code
 */
int lsp_thread_create(lsp_thread_func_t func, 
    const char* name,
    unsigned int stack_size,
    void *parameter,
    unsigned int priority,
    lsp_thread_handle_t *handle);

#endif