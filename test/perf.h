/*
 * perf.h
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jayoun Lee           		<airjany@samsung.com>
 * Contact: Seokkyu Jang                        <seokkyu.jang@samsung.com>
 * Contact: Sangil Yoon                         <si83.yoon@samsung.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef __PERF_H__
#define __PERF_H__

#ifdef PERF_ACTIVATE

#include <sys/time.h>
static struct timeval __g_base_time = {
	.tv_sec = 0,
	.tv_usec = 0
};

#define INIT_PERF(kb)\
do{\
	struct timeval tv;\
	gettimeofday(&tv, NULL);\
	__g_base_time.tv_sec = tv.tv_sec;\
	__g_base_time.tv_usec = tv.tv_usec;\
}while(0);

#define PERF(fmt,arg...)\
do{\
	struct timeval cur;\
	struct timeval res;\
	gettimeofday(&cur,NULL);\
	if(__g_base_time.tv_sec != 0){\
		timersub(&cur, &__g_base_time, &res);\
                printf("%c[1;31m[%s,%d] %u sec %u msec "fmt" %c[0m\n",\
			27,__FUNCTION__,__LINE__,\
				res.tv_sec,res.tv_usec/1000,##arg,27);\
	}\
}while(0);

#else

#define INIT_PERF(kb)
#define PERF(fmt,arg...) 

#endif

#endif
