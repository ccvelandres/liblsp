#ifndef LSP_TYPES_H
#define LSP_TYPES_H

#include <stddef.h>

#define LSP_SUCCESS 0 /** Success */
#define LSP_ERR 1 /** Generic error code */
#define LSP_ERR_NOMEM 2 /** Insufficient resources */
#define LSP_ERR_MUTEX 3 /** Mutex error */
#define LSP_ERR_TIMEOUT 4 /** Timeout */
#define LSP_ERR_INVALID 5 /** Invalid argument */

#define LSP_ERR_QUEUE_FULL 10 /** Queue full */
#define LSP_ERR_QUEUE_EMPTY 11 /** Queue empty */

#endif