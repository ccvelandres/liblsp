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
static inline void lsp_list_add(lsp_list_t *n, lsp_list_head_t *head)
{
    _lsp_list_add(n, head, head->next);
}

/**
 * @brief add a new node to back (useful for queues - fifo)
 * 
 * @param n pointer to node to insert
 * @param head pointer to head
 */
static inline void lsp_list_add_tail(lsp_list_t *n, lsp_list_head_t *head)
{
    _lsp_list_add(n, head->prev, head);
}

/**
 * @brief insert n after node
 * 
 * @param n node to insert
 * @param node positional node
 */
static inline void lsp_list_insert_after(lsp_list_t *n, lsp_list_t *node)
{
    _lsp_list_add(n, node, node->next);
}

/**
 * @brief insert n before node
 * 
 * @param n node to insert
 * @param node positional node
 */
static inline void lsp_list_insert_before(lsp_list_t *n, lsp_list_t *node)
{
    _lsp_list_add(n, node->prev, node);
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
 * @brief checks whether the list is empty
 * 
 */
#define lsp_list_is_empty(head) \
    ((head)->next == (head)->prev ? 1 : 0)

/**
 * @brief iterate through the list
 * 
 * @param ptr the &struct as cursor
 * @param member name of list_t in the struct ptr
 * @param head ptr to list_head
 */
#define lsp_list_for(ptr, member, head)                                                                    \
    for (ptr = ((head)->next != (head)->prev ? container_of((head)->next, typeof(*(ptr)), member) : NULL); \
         ptr;                                                                                              \
         ptr = ((ptr)->member.next != (head) ? container_of((ptr)->member.next, typeof(*(ptr)), member) : NULL))
// for(ptr = (head)->next; ptr != head; ptr = ptr->next)

/**
 * @brief iterator through list backwards
 * 
 * @param ptr the &struct as cursor
 * @param member name of list_t in the struct ptr
 * @param head ptr to list_head
 */
#define lsp_list_for_back(ptr, member, head)                                                               \
    for (ptr = ((head)->next != (head)->prev ? container_of((head)->prev, typeof(*(ptr)), member) : NULL); \
         ptr;                                                                                              \
         ptr = ((ptr)->member.prev != (head) ? container_of((ptr)->member.prev, typeof(*(ptr)), member) : NULL))

#endif