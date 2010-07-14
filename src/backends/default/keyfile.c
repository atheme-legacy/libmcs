/*
 * This is mcs; a modular configuration system.
 *
 * Copyright (c) 2007, 2010 William Pitcock <nenolod -at- atheme.org>
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
#include "keyfile.h"

void nocanon(char *str) {}

keyfile_t *
keyfile_new(void)
{
	keyfile_t *out;

	out = mowgli_alloc(sizeof(keyfile_t));
	out->sections = mowgli_patricia_create(nocanon);

	return out;
}

static void
keyfile_line_free_cb(const char *key, void *data, void *privdata)
{
	free(data);
}

static void
keyfile_section_free_cb(const char *key, void *data, void *privdata)
{
	keyfile_section_t *sec = data;

	mowgli_patricia_destroy(sec->lines, keyfile_line_free_cb, sec);

	free(sec->name);
	mowgli_free(sec);
}

void
keyfile_destroy(keyfile_t *file)
{
	if (file == NULL)
		return;

	mowgli_patricia_destroy(file->sections, keyfile_section_free_cb, file);
	mowgli_free(file);
}

static keyfile_section_t *
keyfile_create_section(keyfile_t *parent, const char *name)
{
	keyfile_section_t *out = mowgli_alloc(sizeof(keyfile_section_t));

	out->name = strdup(name);
	out->lines = mowgli_patricia_create(nocanon);

	mowgli_patricia_add(parent->sections, out->name, out);

	return out;
}

keyfile_t *
keyfile_open(const char *filename)
{
	FILE *f = fopen(filename, "rb");
	keyfile_t *out = keyfile_new();
	keyfile_section_t *sec = NULL;
	char buffer[4096], *tmp;

	if (f == NULL)
		return out;

	while (fgets(buffer, 4096, f))
	{
		if (buffer[0] == '[')
		{
			if ((tmp = strchr(buffer, ']')))
			{
				*tmp = '\0';

				if ((sec = mowgli_patricia_retrieve(out->sections, &buffer[1])) == NULL)
					sec = keyfile_create_section(out, &buffer[1]);
				else
					mowgli_log("Duplicate section %s in %s", &buffer[1], filename);
			}
		}
		else if (buffer[0] != '#' && sec != NULL)
		{
			if ((tmp = strchr(buffer, '=')))
			{
				char *tmp = strtok(buffer, "=");
				char *tmp2 = strtok(NULL, "\n");

				if (tmp2 != NULL && strlen(tmp2) > 0)
				{
					if (mowgli_patricia_retrieve(sec->lines, tmp) == NULL)
						mowgli_patricia_add(sec->lines, tmp, strdup(tmp2));
					else
						mowgli_log("Ignoring duplicate value %s in section %s in %s", tmp, sec->name, filename);
				}
			}
		}
	}

	fclose(f);

	return out;
}

int
keyfile_write_line_cb(const char *key, void *data, void *privdata)
{
	FILE *f = (FILE *) privdata;
	const char *value = data;

	fprintf(f, "%s=%s\n", key, value);

	return 0;
}

int
keyfile_write_section_cb(const char *key, void *data, void *privdata)
{
	FILE *f = (FILE *) privdata;
	keyfile_section_t *sec = data;

	fprintf(f, "[%s]\n", sec->name);
	mowgli_patricia_foreach(sec->lines, keyfile_write_line_cb, f);

	return 0;
}

mcs_response_t
keyfile_write(keyfile_t *self, const char *filename)
{
	FILE *f = fopen(filename, "w+t");

	if (f == NULL)
	{
		mowgli_log("keyfile_write(): Failed to open `%s' for writing: %s",
			filename, strerror(errno));
		return MCS_FAIL;
	}

	mowgli_patricia_foreach(self->sections, keyfile_write_section_cb, f);

	fsync(fileno(f));
	fclose(f);

	return MCS_OK;
}

mcs_response_t
keyfile_get_string(keyfile_t *self, const char *section,
		   const char *key, char **value)
{
	keyfile_section_t *sec;
	const char *val;

	if ((sec = mowgli_patricia_retrieve(self->sections, section)) == NULL)
		return MCS_FAIL;
	if ((val = mowgli_patricia_retrieve(sec->lines, key)) == NULL)
		return MCS_FAIL;

	*value = strdup(val);

	return MCS_OK;
}

mcs_response_t
keyfile_get_int(keyfile_t *self, const char *section,
	        const char *key, int *value)
{
	char *str;

	if (!keyfile_get_string(self, section, key, &str))
		return MCS_FAIL;

	*value = atoi(str);
	free(str);

	return MCS_OK;
}

mcs_response_t
keyfile_get_bool(keyfile_t *self, const char *section,
	         const char *key, int *value)
{
	char *str;

	if (!keyfile_get_string(self, section, key, &str))
		return MCS_FAIL;

	if (!strcasecmp(str, "TRUE"))
		*value = 1;
	else
		*value = 0;
	free(str);

	return MCS_OK;
}

mcs_response_t
keyfile_get_float(keyfile_t *self, const char *section,
	          const char *key, float *value)
{
	char *str, *locale;

	if (!keyfile_get_string(self, section, key, &str))
		return MCS_FAIL;

	locale = strdup(setlocale(LC_NUMERIC, NULL));
	setlocale(LC_NUMERIC, "C");
	*value = strtod(str, NULL);
	setlocale(LC_NUMERIC, locale);

	free(locale);
	free(str);

	return MCS_OK;
}

mcs_response_t
keyfile_get_double(keyfile_t *self, const char *section,
	           const char *key, double *value)
{
	char *str, *locale;

	if (!keyfile_get_string(self, section, key, &str))
		return MCS_FAIL;

	locale = strdup(setlocale(LC_NUMERIC, NULL));
	setlocale(LC_NUMERIC, "C");
	*value = strtod(str, NULL);
	setlocale(LC_NUMERIC, locale);

	free(locale);
	free(str);

	return MCS_OK;
}

mcs_response_t
keyfile_set_string(keyfile_t *self, const char *section,
		   const char *key, const char *value)
{
	keyfile_section_t *sec;
	char *val;

	sec = mowgli_patricia_retrieve(self->sections, section);
	if (sec == NULL)
		sec = keyfile_create_section(self, section);

	if ((val = mowgli_patricia_retrieve(sec->lines, key)) != NULL)
	{
		free(val);

		mowgli_patricia_delete(sec->lines, key);
	}

	mowgli_patricia_add(sec->lines, key, strdup(value));

	return MCS_OK;
}

mcs_response_t
keyfile_set_int(keyfile_t *self, const char *section,
		const char *key, int value)
{
	char strval[4096];

	snprintf(strval, 4096, "%d", value);
	keyfile_set_string(self, section, key, strval);

	return MCS_OK;
}

mcs_response_t
keyfile_set_bool(keyfile_t *self, const char *section,
		 const char *key, int value)
{
	if (value)
		keyfile_set_string(self, section, key, "TRUE");
	else
		keyfile_set_string(self, section, key, "FALSE");

	return MCS_OK;
}

mcs_response_t
keyfile_set_float(keyfile_t *self, const char *section,
		  const char *key, float value)
{
	char strval[4096];
	char *locale;

	locale = strdup(setlocale(LC_NUMERIC, NULL));
	setlocale(LC_NUMERIC, "C");
	snprintf(strval, 4096, "%g", value);
	setlocale(LC_NUMERIC, locale);
	keyfile_set_string(self, section, key, strval);
	free(locale);

	return MCS_OK;
}

mcs_response_t
keyfile_set_double(keyfile_t *self, const char *section,
		   const char *key, double value)
{
	char strval[4096];
	char *locale;

	locale = strdup(setlocale(LC_NUMERIC, NULL));
	setlocale(LC_NUMERIC, "C");
	snprintf(strval, 4096, "%g", value);
	setlocale(LC_NUMERIC, locale);
	keyfile_set_string(self, section, key, strval);
	free(locale);

	return MCS_OK;
}

mcs_response_t
keyfile_unset_key(keyfile_t *self, const char *section,
		  const char *key)
{
	keyfile_section_t *sec;
	char *value;

	if ((sec = mowgli_patricia_retrieve(self->sections, section)) != NULL)
	{
		if ((value = mowgli_patricia_retrieve(sec->lines, key)) != NULL)
			free(value);

		mowgli_patricia_delete(sec->lines, key);
	}

	return MCS_OK;
}

/* ***************************************************************** */

extern mcs_backend_t mcs_backend;

typedef struct {
	char *loc;
	keyfile_t *kf;
} mcs_keyfile_handle_t;

mcs_handle_t *
mcs_keyfile_new(char *domain)
{
	char scratch[PATH_MAX];
	char *magic = getenv("XDG_CONFIG_HOME");

#if defined(__WIN32) || defined(__MINGW32__)
	const mode_t mode755 = 0;
#else
	const mode_t mode755 = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH |
			       S_IXOTH;
#endif

	mcs_keyfile_handle_t *h = calloc(sizeof(mcs_keyfile_handle_t), 1);
	mcs_handle_t *out = calloc(sizeof(mcs_handle_t), 1);

	out->base = &mcs_backend;
	out->mcs_priv_handle = h;

	if (magic != NULL)
		snprintf(scratch, PATH_MAX, "%s/%s", magic, domain);
	else
		snprintf(scratch, PATH_MAX, "%s/.config/%s", getenv("HOME"), domain);

	mcs_create_directory(scratch, mode755);
	mcs_strlcat(scratch, "/config", PATH_MAX);

	h->loc = strdup(scratch);
	h->kf = keyfile_open(h->loc);

	return out;
}

void
mcs_keyfile_destroy(mcs_handle_t *self)
{
	char tfile[PATH_MAX];
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return_if_fail(h->kf != NULL);
	return_if_fail(h->loc != NULL);

	mcs_strlcpy(tfile, h->loc, PATH_MAX);
	mcs_strlcat(tfile, ".tmp", PATH_MAX);

	keyfile_write(h->kf, tfile);
	keyfile_destroy(h->kf);

	rename(tfile, h->loc);

	free(h->loc);
	free(h);

	free(self);
}

mcs_response_t
mcs_keyfile_get_string(mcs_handle_t *self, const char *section,
		       const char *key, char **value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_get_string(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_get_int(mcs_handle_t *self, const char *section,
		    const char *key, int *value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_get_int(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_get_bool(mcs_handle_t *self, const char *section,
		     const char *key, int *value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_get_bool(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_get_float(mcs_handle_t *self, const char *section,
		      const char *key, float *value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_get_float(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_get_double(mcs_handle_t *self, const char *section,
		       const char *key, double *value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_get_double(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_set_string(mcs_handle_t *self, const char *section,
		       const char *key, const char *value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_set_string(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_set_int(mcs_handle_t *self, const char *section,
		    const char *key, int value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_set_int(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_set_bool(mcs_handle_t *self, const char *section,
		     const char *key, int value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_set_bool(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_set_float(mcs_handle_t *self, const char *section,
		      const char *key, float value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_set_float(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_set_double(mcs_handle_t *self, const char *section,
		       const char *key, double value)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_set_double(h->kf, section, key, value);
}

mcs_response_t
mcs_keyfile_unset_key(mcs_handle_t *self, const char *section,
		      const char *key)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;

	return keyfile_unset_key(h->kf, section, key);
}

int
keyfile_collect_lines_cb(const char *key, void *data, void *privdata)
{
	mowgli_queue_t **out = privdata;
	mowgli_queue_t *tmp;

	tmp = mowgli_queue_shift(*out, strdup(key));
	*out = tmp;

	return 0;
}

mowgli_queue_t *
mcs_keyfile_get_keys(mcs_handle_t *self, const char *section)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;
	keyfile_section_t *ks = mowgli_patricia_retrieve(h->kf->sections, section);
	mowgli_queue_t *out = NULL;

	if (ks == NULL)
		return NULL;

	mowgli_patricia_foreach(ks->lines, keyfile_collect_lines_cb, &out);

	return out;
}

mowgli_queue_t *
mcs_keyfile_get_sections(mcs_handle_t *self)
{
	mcs_keyfile_handle_t *h = (mcs_keyfile_handle_t *) self->mcs_priv_handle;
	mowgli_queue_t *out = NULL;

	mowgli_patricia_foreach(h->kf->sections, keyfile_collect_lines_cb, &out);

	return out;
}

mcs_backend_t mcs_backend = {
	NULL,
	"default",
	mcs_keyfile_new,
	mcs_keyfile_destroy,

	mcs_keyfile_get_string,
	mcs_keyfile_get_int,
	mcs_keyfile_get_bool,
	mcs_keyfile_get_float,
	mcs_keyfile_get_double,

	mcs_keyfile_set_string,
	mcs_keyfile_set_int,
	mcs_keyfile_set_bool,
	mcs_keyfile_set_float,
	mcs_keyfile_set_double,

	mcs_keyfile_unset_key,

	mcs_keyfile_get_keys,
	mcs_keyfile_get_sections
};
