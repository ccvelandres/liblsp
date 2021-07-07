#ifndef LSP_LIST_H
#define LSP_LIST_H

#include "lsp_memory.h"

typedef struct lsp_list_s
{
    struct lsp_list_s *next, *prev;
} lsp_list_t;

typedef lsp_list_t lsp_list_head_t;

#define LSP_LIST_HEAD_INIT(name) \
    {                            \
        &(name), &(name)         \
    }
#define LSP_LIST_HEAD(name) \
    lsp_list_head_t name = LSP_LIST_HEAD_INIT(name)

/**
 * @brief initializes the list head.
 * Use LSP_LIST_HEAD_INIT for static list heads
 * 
 * @param head pointer to list head
 */
static inline void lsp_list_head_init(lsp_list_head_t *head)
{
    head->next = head;
    head->prev = head;
}

/**
 * @brief internal use only! when we know prev/next nodes
 * 
 * @param n pointer to node to insert
 * @param prev pointer to prev node
 * @param next pointer to next node
 */
static inline void _lsp_list_add(lsp_list_t *n, lsp_list_t *prev, lsp_list_t *next)
{
    next->prev = n;
    n->next = next;
    n->prev = prev;
    prev->next = n;
}

/**
 * @brief add a new node to front (useful for stacks - lifo)
 * 
 * @param n pointer to node to insert
 * @param head pointer to head
 */
static inline void lsp_list_add(lsp_list_t *n, lsp_list_t *head)
{
    _lsp_list_add(n, head, head->next);
}

/**
 * @brief add a new node to back (useful for queues - fifo)
 * 
 * @param n pointer to node to insert
 * @param head pointer to head
 */
static inline void lsp_list_add_tail(lsp_list_t *n, lsp_list_t *head)
{
    _lsp_list_add(n, head->prev, head);
}

/**
 * @brief removes node from list
 * 
 * @param n pointer to node to remove
 */
static inline void lsp_list_del(lsp_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
}

/**
 * @brief iterate through the list
 * 
 * @param ptr the &struct list_head as cursor
 * @param member name of list_head in the struct ptr
 * @param head the head of the list
 */
#define lsp_list_for(ptr, member, head)                                                                    \
    for (ptr = ((head)->next != (head)->prev ? container_of((head)->next, typeof(*(ptr)), member) : NULL); \
         ptr;                                                                                              \
         ptr = ((ptr)->member.next != (head) ? container_of((ptr)->member.next, typeof(*(ptr)), member) : NULL))
// for(ptr = (head)->next; ptr != head; ptr = ptr->next)

/**
 * @brief iterator through list backwards
 * 
 * @param ptr the struct list_head
 * @param head the head of the list
 */
#define lsp_list_for_back(ptr, member, head)                                                               \
    for (ptr = ((head)->next != (head)->prev ? container_of((head)->prev, typeof(*(ptr)), member) : NULL); \
         ptr;                                                                                              \
         ptr = ((ptr)->member.prev != (head) ? container_of((ptr)->member.prev, typeof(*(ptr)), member) : NULL))

#endif