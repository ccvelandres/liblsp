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

#include "lsp_thread.h"
#include "lsp_log.h"

static const char *tag = "lsp_thread";

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
        lsp_verb(tag, "could not create pthread %d\n", rc);
        return LSP_ERR;
    }
    else 
        return LSP_ERR_NONE;
}