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

#include "lsp_buffer.h"
#include "lsp_memory.h"
#include "lsp_log.h"

#include "stdio.h"

static const char *tag = "lsp_buffer";

lsp_buffer_t *lsp_buffer_alloc(lsp_interface_t *iface, size_t len)
{
    size_t headroom = (iface != NULL ? iface->min_header_len : LSP_DEFAULT_BUFFER_HEADER_LEN);
    lsp_buffer_t *buff = lsp_malloc(ALIGNED_SIZEOF(lsp_buffer_t) + len + headroom);
    lsp_verb(tag, "%s: %p struct size %d buff size %d 0x%x\n",
             __FUNCTION__, buff, ALIGNED_SIZEOF(lsp_buffer_t), ALIGNED_SIZEOF(lsp_buffer_t) + len + headroom, ALIGNED_SIZEOF(lsp_buffer_t) + len + headroom);
    if (buff == NULL)
    {
        lsp_dbg(tag, "could not allocate lsp_buffer\n");
        return NULL;
    }

    buff->iface = iface;
    buff->head = ((unsigned char *)buff + ALIGNED_SIZEOF(lsp_buffer_t));
    buff->data = buff->tail = buff->head + headroom;
    buff->end = buff->head + len + headroom;
    buff->headroom = headroom;
    buff->tailroom = len;
    buff->lsp_packet = (typeof(buff->lsp_packet))buff->data;
    return buff;
}

void *lsp_buffer_put(lsp_buffer_t *buff, size_t len)
{
    if (buff->tailroom < len)
    {
        lsp_dbg(tag, "%s: fail, overflow\n", __FUNCTION__);
        return NULL;
    }
    void *data = buff->tail;
    buff->tailroom -= len;
    buff->tail += len;
    return data;
}

void *lsp_buffer_push(lsp_buffer_t *buff, size_t len)
{
    if (buff->headroom < len)
    {
        lsp_dbg(tag, "%s: fail, overflow\n", __FUNCTION__);
        return NULL;
    }
    buff->headroom -= len;
    buff->data -= len;
    return buff->data;
}

void *lsp_buffer_pull(lsp_buffer_t *buff, size_t len)
{
    // clip len to max len if it exceeds
    if (buff->tail - buff->data < len)
        len = buff->tail - buff->data;

    void *data = buff->data;
    buff->data += len;
    buff->headroom += len;

    if (buff->data > buff->tail)
    {
        buff->tailroom -= buff->data - buff->tail;
        buff->tail = buff->data;
    }

    return data;
}

int lsp_buffer_free(lsp_buffer_t *buff)
{
    lsp_free(buff);
    return LSP_ERR_NONE;
}

void lsp_buffer_debug(lsp_buffer_t *buff)
{
    lsp_dbg(tag, "lsp_buffer @      ----------  0x%08x  ----------\n", buff);
    lsp_dbg(tag, "  iface         = 0x%08x  0x%08x  0x%08x\n", buff->iface, &buff->iface, (void *)(&buff->iface) - (void *)buff);
    lsp_dbg(tag, "  list->next    = 0x%08x  0x%08x  0x%08x\n", buff->list.next, &buff->list.next, (void *)&(buff->list) - (void *)buff);
    lsp_dbg(tag, "  list->prev    = 0x%08x  0x%08x  0x%08x\n", buff->list.prev, &buff->list.prev, (void *)&(buff->list) - (void *)buff);
    lsp_dbg(tag, "  data          = 0x%08x  0x%08x  0x%08x\n", buff->data, &buff->data, (void *)(&buff->data) - (void *)buff);
    lsp_dbg(tag, "  tail          = 0x%08x  0x%08x  0x%08x\n", buff->tail, &buff->tail, (void *)(&buff->tail) - (void *)buff);
    lsp_dbg(tag, "  end           = 0x%08x  0x%08x  0x%08x\n", buff->end, &buff->end, (void *)(&buff->end) - (void *)buff);
    lsp_dbg(tag, "  lsp_packet    = 0x%08x  0x%08x  0x%08x\n", buff->lsp_packet, &buff->lsp_packet, (void *)(&buff->lsp_packet) - (void *)buff);
    lsp_dbg(tag, "  head          = 0x%08x  0x%08x  0x%08x\n", buff->head, &buff->head, (void *)(&buff->head) - (void *)buff);
}