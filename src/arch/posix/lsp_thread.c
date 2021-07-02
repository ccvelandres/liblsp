#include "lsp_thread.h"
#include "lsp_log.h"

int lsp_thread_create(lsp_thread_func_t func,
                      const char *name,
                      unsigned int stack_size,
                      void *parameter,
                      unsigned int priority,
                      lsp_thread_handle_t *handle)
{
    // create thread with default attr
    int rc = pthread_create(handle, NULL, func, parameter);
    if(rc)
    {
        lsp_verb("could not create pthread %d\n", rc);
        return LSP_ERR;
    }
    else 
        return LSP_SUCCESS;
}