/*
 * This is mcs; a modular configuration system.
 *
 * Copyright (c) 2007 William Pitcock <nenolod -at- sacredspiral.co.uk>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __LIBMCS_MCS_H__
#define __LIBMCS_MCS_H__

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <errno.h>

#include <libmcs/mcs_config.h>

typedef enum {
	MCS_FAIL, MCS_OK
} mcs_response_t;

typedef struct mcs_handle_ mcs_handle_t;

typedef struct {
	void *handle;
	char *name;

	/* constructors and destructors */
	mcs_handle_t *(*mcs_new)(char *domain);
	void (*mcs_destroy)(mcs_handle_t *handle);

	/* retrieval */
	mcs_response_t (*mcs_get_string)(mcs_handle_t *handle,
					 const char *section,
					 const char *key,
					 char **value);

	mcs_response_t (*mcs_get_int)(mcs_handle_t *handle,
				      const char *section,
				      const char *key,
				      int *value);

	mcs_response_t (*mcs_get_bool)(mcs_handle_t *handle,
				       const char *section,
				       const char *key,
				       int *value);

	mcs_response_t (*mcs_get_float)(mcs_handle_t *handle,
				        const char *section,
				        const char *key,
				        float *value);

	mcs_response_t (*mcs_get_double)(mcs_handle_t *handle,
				         const char *section,
				         const char *key,
				         double *value);

	/* setting data */
	mcs_response_t (*mcs_set_string)(mcs_handle_t *handle,
					 const char *section,
					 const char *key,
					 const char *value);

	mcs_response_t (*mcs_set_int)(mcs_handle_t *handle,
				      const char *section,
				      const char *key,
				      int value);

	mcs_response_t (*mcs_set_bool)(mcs_handle_t *handle,
				       const char *section,
				       const char *key,
				       int value);

	mcs_response_t (*mcs_set_float)(mcs_handle_t *handle,
				        const char *section,
				        const char *key,
				        float value);

	mcs_response_t (*mcs_set_double)(mcs_handle_t *handle,
				         const char *section,
				         const char *key,
				         double value);

	/* unset */
	mcs_response_t (*mcs_unset_key)(mcs_handle_t *handle,
					const char *section,
					const char *key);
} mcs_backend_t;

struct mcs_handle_ {
	mcs_backend_t *base;
	void *mcs_priv_handle;
};

typedef struct mcs_list_ {
	struct mcs_list_ *prev;
	struct mcs_list_ *next;
	void *data;
} mcs_list_t;

/*
 * These functions have to do with initialization of the
 * library.
 */
extern void  mcs_init(void);
extern void  mcs_fini(void);
extern char *mcs_version(void);
 
/*
 * These functions have to do with registration of MCS backends.
 */
extern mcs_response_t mcs_backend_register(mcs_backend_t *backend);
extern mcs_response_t mcs_backend_unregister(mcs_backend_t *backend);
extern mcs_list_t *   mcs_backend_get_list(void);
extern char *         mcs_backend_select(void);

/*
 * These functions provide the public interface for creating and closing MCS
 * handles.
 *
 * Please note that if a handle is not closed, the data may not be saved to
 * disk.
 */
extern mcs_handle_t *mcs_new(char *domain);
extern void mcs_destroy(mcs_handle_t *handle);

/*
 * These functions provide the public interface for querying and setting data.
 */
/* retrieval */
extern mcs_response_t mcs_get_string(mcs_handle_t *handle,
				 const char *section,
				 const char *key,
				 char **value);

extern mcs_response_t mcs_get_int(mcs_handle_t *handle,
			      const char *section,
			      const char *key,
			      int *value);

extern mcs_response_t mcs_get_bool(mcs_handle_t *handle,
			       const char *section,
			       const char *key,
			       int *value);

extern mcs_response_t mcs_get_float(mcs_handle_t *handle,
			        const char *section,
			        const char *key,
			        float *value);

extern mcs_response_t mcs_get_double(mcs_handle_t *handle,
			         const char *section,
			         const char *key,
			         double *value);

/* setting data */
extern mcs_response_t mcs_set_string(mcs_handle_t *handle,
				 const char *section,
				 const char *key,
				 const char *value);

extern mcs_response_t mcs_set_int(mcs_handle_t *handle,
			      const char *section,
			      const char *key,
			      int value);

extern mcs_response_t mcs_set_bool(mcs_handle_t *handle,
			       const char *section,
			       const char *key,
			       int value);

extern mcs_response_t mcs_set_float(mcs_handle_t *handle,
			        const char *section,
			        const char *key,
			        float value);

extern mcs_response_t mcs_set_double(mcs_handle_t *handle,
			         const char *section,
			         const char *key,
			         double value);

/* unset */
extern mcs_response_t mcs_unset_key(mcs_handle_t *handle,
				const char *section,
				const char *key);

/*
 * These functions have to do with the plugin loader.
 */
extern void mcs_load_plugins(void);
extern void mcs_unload_plugins(mcs_list_t *l);

/*
 * These functions manipulate linked lists.
 */
extern mcs_list_t *mcs_list_append(mcs_list_t *head, void *data);
extern mcs_list_t *mcs_list_remove(mcs_list_t *head);
extern mcs_list_t *mcs_list_find(mcs_list_t *head, void *data);
extern mcs_list_t *mcs_list_remove_data(mcs_list_t *head, void *data);
extern void mcs_list_free(mcs_list_t *head);

/*
 * These functions have to do with logging.
 */
typedef void (*mcs_log_func_t)(const char *text);
extern void mcs_log(const char *fmt, ...);
extern void mcs_log_set_log_func(mcs_log_func_t func);

/*
 * These functions are utility functions.
 */
extern size_t mcs_strnlen(const char *str, size_t len);
extern char * mcs_strndup(const char *str, size_t len);
extern int mcs_create_directory(const char *path, mode_t mode);

#endif
