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

#include "libmcs/mcs.h"

#include <locale.h>

#ifndef __KEYFILE_H__
#define __KEYFILE_H__

typedef struct {
	char *key;
	char *value;
} keyfile_line_t;

typedef struct {
	char *name;
	mcs_list_t *lines;
} keyfile_section_t;

typedef struct {
	mcs_list_t *sections;
} keyfile_t;

#ifdef __cplusplus
extern "C" {
#endif

extern keyfile_t *keyfile_new(void);
extern void keyfile_destroy(keyfile_t *keyfile);

extern keyfile_t *keyfile_open(const char *filename);
extern mcs_response_t keyfile_write(keyfile_t *keyfile, const char *filename);

extern mcs_response_t keyfile_get_string(keyfile_t *keyfile,
					 const char *section, const char *key,
					 char **value);
extern mcs_response_t keyfile_get_int(keyfile_t *keyfile,
				      const char *section, const char *key,
				      int *value);
extern mcs_response_t keyfile_get_bool(keyfile_t *keyfile,
				       const char *section, const char *key,
				       int *value);
extern mcs_response_t keyfile_get_float(keyfile_t *keyfile,
				        const char *section, const char *key,
				        float *value);
extern mcs_response_t keyfile_get_double(keyfile_t *keyfile,
				         const char *section, const char *key,
				         double *value);

extern mcs_response_t keyfile_set_string(keyfile_t *keyfile,
					 const char *section, const char *key,
					 const char *value);
extern mcs_response_t keyfile_set_int(keyfile_t *keyfile,
				      const char *section, const char *key,
				      int value);
extern mcs_response_t keyfile_set_bool(keyfile_t *keyfile,
				       const char *section, const char *key,
				       int value);
extern mcs_response_t keyfile_set_float(keyfile_t *keyfile,
				        const char *section, const char *key,
				        float value);
extern mcs_response_t keyfile_set_double(keyfile_t *keyfile,
				         const char *section, const char *key,
				         double value);

extern mcs_response_t keyfile_remove_key(keyfile_t *keyfile,
				         const char *section, const char *key);

#ifdef __cplusplus
};
#endif

#endif
