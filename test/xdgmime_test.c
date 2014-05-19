/*
 * xdgmime_test.c
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

#include <stdio.h>
#include "xdgmime/src/xdgmime.h"

#define PERF_ACTIVATE
#include "perf.h"

int main(int argc, char** argv)
{
	const char* res;
	if(argc < 2){
		printf("[usage] %s <filename> or <mimetype>\n",argv[0]);
		exit(0);
	}

	printf("unaliased = %s\n",xdg_mime_unalias_mime_type(argv[1]));

	INIT_PERF("start");
	res = xdg_mime_get_mime_type_for_file(argv[1],NULL);
	if(strcmp(res,"application/octet-stream")==0)
		res = xdg_mime_get_mime_type_from_file_name(argv[1]);
	PERF("end");
	printf("%s has a mime-type of %s\n", argv[1], res);

	return 0;
}

/* vi: set ts=8 sts=8 sw=8: */
