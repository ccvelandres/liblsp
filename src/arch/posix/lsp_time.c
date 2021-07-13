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
#include "lsp_time.h"

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <locale.h>

#include "string.h"

static const char *tag = "lsp_time";

uint32_t lsp_uptime()
{
    FILE *fp;
    double uptime = 0;
    fp = fopen("/proc/uptime", "r");
    if (fp != NULL)
    {
        char buf[32];
        int res;
        char *b = fgets(buf, 32, fp);
        if (b == buf)
        {
            /* The following sscanf must use the C locale.  */
            setlocale(LC_NUMERIC, "C");
            res = sscanf(buf, "%lf", &uptime);
            setlocale(LC_NUMERIC, "");
            if (res != 0)
                uptime = 0;
        }
        fclose(fp);
    }
end:
    return (uint32_t)uptime;
}

uint32_t lsp_gettime_ms()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts))
        return 0;

    return (((uint32_t)(ts.tv_sec)) * 1000) + (((uint32_t)(ts.tv_nsec)) * 1000000);
}

uint32_t lsp_gettime_s()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts))
        return 0;

    return (((uint32_t)(ts.tv_sec)) * 1000);
}