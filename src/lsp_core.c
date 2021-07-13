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

#include "lsp.h"
#include "lsp_port.h"
#include "lsp_memory.h"
#include "lsp_conn.h"
#include "lsp_log.h"
#include "lsp_thread.h"

#include "string.h"

static const char *tag = "lsp_core";

struct lsp_core_event {
    lsp_events_t ev;
    void *data;
};

/** LSP Core thread handle */
static lsp_thread_handle_t lsp_core_thread;
/** LSP Core Event Queue */
static lsp_queue_handle_t lsp_core_evqueue;

lsp_thread_return_t lsp_core_task(void *arg);

int lsp_core_start()
{
    /** TODO: maybe add support for multithreaded core task */
    int rc;
    
    lsp_core_evqueue = lsp_queue_create(LSP_DEFAULT_CORE_EVQUEUE_LEN, sizeof(struct lsp_core_event));
    if(lsp_core_evqueue == NULL)
    {
        lsp_err(tag, "%s: failed to create event queue\n", __FUNCTION__);
        goto end;
    }

    // create with 2048 stack size
    rc = lsp_thread_create(lsp_core_task, "lsp_core", 2048, NULL, LSP_DEFAULT_CORE_PRIORITY, &lsp_core_thread);
    if (rc != LSP_ERR_NONE)
    {
        lsp_err(tag, "%s: failed to create thread\n", __FUNCTION__);
        goto queue_err;
    }

queue_err:
    lsp_queue_destroy(lsp_core_evqueue);
end:
    return rc;
}

int lsp_core_handle_rxev(void * data)
{
    /** TODO: Handle rx packets here */
}

lsp_thread_return_t lsp_core_task(void *arg)
{
    int rc;
    struct lsp_core_event event;
    uint32_t nextSleep = 500; /** TODO: do calculation to get next sleep time */
    for (;;)
    {
        rc = lsp_queue_pop(lsp_core_evqueue, &event, nextSleep);
        if(rc == LSP_ERR_TIMEOUT || rc == LSP_ERR_QUEUE_EMPTY)
        {
            event.ev = LSP_EV_NO_EVENT;
        }
        else if (rc != LSP_ERR_NONE){
            event.ev = LSP_EV_NO_EVENT;
            lsp_err(tag, "%s: event queue error %d\n", __FUNCTION__, rc);
        }

        switch(event.ev)
        {
            case LSP_EV_NO_EVENT:
                break;
            case LSP_EV_NET_RX_EVENT:

                break;
        }
    }
}

int lsp_core_sendevent(lsp_events_t ev, void *data)
{
    int rc;
    struct lsp_core_event event = {
        .ev = ev,
        .data = data
    };

    rc = lsp_queue_push(lsp_core_evqueue, &event, 0);
    if(rc != LSP_ERR_NONE)
    {
        lsp_verb(tag, "%s: could not push to evqueue err: %d\n", __FUNCTION__, rc);
    }
    return rc;
}