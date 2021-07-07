#include "lsp_buffer.h"
#include "lsp_memory.h"
#include "lsp_log.h"

#include "stdio.h"

lsp_buffer_t *lsp_buffer_alloc(lsp_interface_t *iface, size_t len)
{
    size_t headroom = (iface != NULL ? iface->min_header_len : LSP_DEFAULT_BUFFER_HEADER_LEN);
    lsp_buffer_t *buff = lsp_malloc(ALIGNED_SIZEOF(lsp_buffer_t) + len + headroom);
    lsp_verb("lsp_buffer_alloc() %p struct size %d buff size %d 0x%x\n", buff, ALIGNED_SIZEOF(lsp_buffer_t), ALIGNED_SIZEOF(lsp_buffer_t) + len + headroom, ALIGNED_SIZEOF(lsp_buffer_t) + len + headroom);
    if (buff == NULL)
    {
        lsp_dbg("could not allocate lsp_buffer\n");
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
        lsp_dbg("lsp_buffer_put() fail, overflow\n");
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
        lsp_dbg("lsp_buffer_push() fail, overflow\n");
        return NULL;
    }
    buff->headroom -= len;
    buff->data -= len;
    return buff->data;
}

void lsp_buffer_debug(lsp_buffer_t *buff)
{
    lsp_dbg("lsp_buffer @      ----------  0x%08x  ----------\n", buff);
    lsp_dbg("  iface         = 0x%08x  0x%08x  0x%08x\n", buff->iface, &buff->iface, (void *)(&buff->iface) - (void *)buff);
    lsp_dbg("  list->next    = 0x%08x  0x%08x  0x%08x\n", buff->list.next, &buff->list.next, (void *)&(buff->list) - (void *)buff);
    lsp_dbg("  list->prev    = 0x%08x  0x%08x  0x%08x\n", buff->list.prev, &buff->list.prev, (void *)&(buff->list) - (void *)buff);
    lsp_dbg("  data          = 0x%08x  0x%08x  0x%08x\n", buff->data, &buff->data, (void *)(&buff->data) - (void *)buff);
    lsp_dbg("  tail          = 0x%08x  0x%08x  0x%08x\n", buff->tail, &buff->tail, (void *)(&buff->tail) - (void *)buff);
    lsp_dbg("  end           = 0x%08x  0x%08x  0x%08x\n", buff->end, &buff->end, (void *)(&buff->end) - (void *)buff);
    lsp_dbg("  lsp_packet    = 0x%08x  0x%08x  0x%08x\n", buff->lsp_packet, &buff->lsp_packet, (void *)(&buff->lsp_packet) - (void *)buff);
    lsp_dbg("  head          = 0x%08x  0x%08x  0x%08x\n", buff->head, &buff->head, (void *)(&buff->head) - (void *)buff);
}