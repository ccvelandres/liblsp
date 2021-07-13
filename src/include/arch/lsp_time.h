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

#ifndef LSP_TIME_H
#define LSP_TIME_H

#include <stddef.h>
#include "lsp_types.h"

/**
 * @brief returns the uptime of system in seconds
 * 
 * @return uint32_t uptime in seconds
 */
uint32_t lsp_uptime();

/**
 * @brief returns current time in ms
 * 
 * @return uint32_t time in ms
 */
uint32_t lsp_gettime_ms();

/**
 * @brief returns current time in seconds
 * 
 * @return uint32_t time in seconds
 */
uint32_t lsp_gettime_s();

#endif