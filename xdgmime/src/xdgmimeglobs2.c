/*
 * xdgmimeglobs2.c
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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "xdgmime.h"

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif


#define GLOBS2_PATH "/usr/share/mime/globs2"
#define MEM_INC_RATE 20


/*************
 * Data types
 *************/
typedef struct mime_type_info
{
	char *mime_type;
	char **file_names;
	size_t file_names_size;
} mime_type_info;

typedef struct mime_type_info_list
{
	mime_type_info **mti_list;
	size_t mti_list_size;
	time_t globs2_mtime;
} mime_type_info_list;

static mime_type_info_list *mti_list = NULL;


/***************************
 * mime_type_info functions
 ***************************/
static mime_type_info *
mime_type_info_new(void)
{
	mime_type_info *mti = NULL;

	mti = calloc(1, sizeof(mime_type_info));
	mti->file_names = calloc(MEM_INC_RATE, sizeof(char *));
	mti->file_names_size = MEM_INC_RATE;

	return mti;
}

static void
mime_type_info_free(mime_type_info *mti)
{
	if(!mti) return;

	if(mti->mime_type) free(mti->mime_type);
	if(mti->file_names) {
		char **tmpstr;
		for(tmpstr = mti->file_names; *tmpstr; tmpstr++) {
			free(*tmpstr);
		}
		free(mti->file_names);
	}
	free(mti);
}

static int
mime_type_info_add_file_name(mime_type_info *mti,
		const char *mime_type,
		const char *file_name)
{
	if(!mti) return;

	if(!mti->mime_type) {
		mti->mime_type = strdup(mime_type);
	}
	else { 
		/* mime_type already exist, but mime_type is different! */
		if(strcmp(mti->mime_type, mime_type)) return -1;
	}

	/* Find position */
	char **pname;
	for(pname = mti->file_names; pname < (mti->file_names + mti->file_names_size - 1) && *pname; pname++) {
		if(!strcmp(file_name, *pname)) return 0;	/* already exist! */
	}

	int old_file_names_size = mti->file_names_size;

	if(pname == mti->file_names + old_file_names_size -1) {

		/* memory full. realloc needed */
		mti->file_names = realloc(mti->file_names, sizeof(char *)*(old_file_names_size+MEM_INC_RATE));
		if(!mti->file_names) return -1;

		memset(mti->file_names + old_file_names_size, 0x00, sizeof(char *)*MEM_INC_RATE);
		pname = mti->file_names + old_file_names_size - 1;
		mti->file_names_size += MEM_INC_RATE;
	}

	*pname = strdup(file_name);
	
	return 0;
}



/********************************
 * mime_type_info_list functions
 ********************************/

/* definitions */

static mime_type_info_list *
mime_type_info_list_new(void)
{
	mime_type_info_list *mtil;

	mtil = calloc(1, sizeof(mime_type_info_list));

	mtil->mti_list = calloc(MEM_INC_RATE, sizeof(mime_type_info *));
	mtil->mti_list_size = MEM_INC_RATE;

	return mtil;
}



/* free and init mtlist */
static void
mime_type_info_list_free(mime_type_info_list *mtil)
{
	mime_type_info **mti;
	char *tmp;

	for(mti = mtil->mti_list; 
			*mti; 
			mti++) {
		mime_type_info_free(*mti);
	}

	free(mtil->mti_list);
	free(mtil);
}

static int
mime_type_info_list_add_file_name(mime_type_info_list *mtil,
		const char *mime_type,
		const char *file_name)
{
	if(!mtil) return;

	mime_type_info **mti;
	int found = 0;

	for(mti = mtil->mti_list; mti < (mtil->mti_list + mtil->mti_list_size - 1) && *mti; mti++) {
		if((*mti)->mime_type &&	mime_type &&  /* NULL check */
			0 == strncmp((*mti)->mime_type, 
				mime_type, 
				strlen(mime_type))) {
			/* found! */
			found = 1;
			break;
		}
	}
	if(!found) {
		/* New mime_type_info must be needed */

		/* check memory full */
		int old_mti_list_size = mtil->mti_list_size;
		//printf("sizeof(mti_list)=%d, sizeof(mime_type_info *)=%d, old info_list_size=%d\n", sizeof(mtil->mti_list), sizeof(mime_type_info *), old_mti_list_size);

		/* last chance */
		if(mti == mtil->mti_list + old_mti_list_size - 1) {
			/* memory full. realloc needed */
			mtil->mti_list = realloc(mtil->mti_list, sizeof(mime_type_info *) * (old_mti_list_size + MEM_INC_RATE));
			if(!mtil->mti_list) return -1;
			memset(mtil->mti_list + old_mti_list_size, 0x0, sizeof(mime_type_info *) * MEM_INC_RATE);
			mti = mtil->mti_list + old_mti_list_size - 1;

			mtil->mti_list_size += MEM_INC_RATE;
		}
	}

	/* assign new data */
	if(NULL == *mti) *mti = mime_type_info_new();
	mime_type_info_add_file_name(*mti, mime_type, file_name);

	return 0;
}


/* create or renew mtlist */
static void
mime_type_info_list_reload(mime_type_info_list *mtil)
{
	FILE *globs2 = NULL;
	struct stat globs2_stat;
	int r;
	char buf[256];

	if(!mtil) return;

	/* Check glob2's mtime.
	 * If reconstruction is not needed, just exit function */
	if( stat(GLOBS2_PATH, &globs2_stat) ||
			globs2_stat.st_mtime <= mtil->globs2_mtime ) return;

	/* clean old mtil */
	mime_type_info **mti;
	for(mti = mtil->mti_list; 
		*mti; 
		mti++) {
		mime_type_info_free(*mti);
		*mti = NULL;
	}

	/* save globs2's mtime */
	mtil->globs2_mtime = globs2_stat.st_mtime;

	/* read globs2, and construct data structure */
	globs2 = fopen(GLOBS2_PATH, "r");
	char *weight, *mime_type, *file_name, *saveptr = NULL;
	while(fgets(buf, 255, globs2)) {
		/* skip comment */
		if(*buf == '#') continue;
		/* weight:mime_type:file_name */
		weight = strtok_r(buf, ":\n", &saveptr);	/* ignored */
		mime_type = strtok_r(NULL, ":\n", &saveptr);
		file_name = strtok_r(NULL, ":\n", &saveptr);
		
		mime_type_info_list_add_file_name(mtil, mime_type, file_name);
	}
	fclose(globs2);

	/* TODO: sort it! */

}

static const char **
mime_type_info_list_get_file_names(mime_type_info_list *mtil, 
		const char *mime_type)
{

	static const char *null_array[] = { NULL };

	if(!mtil) return null_array;

	mime_type_info **_mti;
	for(_mti = mtil->mti_list; *_mti; _mti++) {
		if((*_mti)->mime_type && mime_type &&   /* NULL check */
			!strcmp((*_mti)->mime_type, mime_type)) {
			return (const char **) (*_mti)->file_names;
		}
	}

	return null_array;
}


/* API 
 * Get file names' list from mime type
 */
API const char **
xdg_mime_get_file_names_from_mime_type(const char *mime_type)
{
	/* init data structure */
	static mime_type_info_list *mtil = NULL;
	if(!mtil) mtil = mime_type_info_list_new();
	mime_type_info_list_reload(mtil);

	return mime_type_info_list_get_file_names(mtil, mime_type);
}

