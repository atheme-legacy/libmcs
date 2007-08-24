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
#include "gconf.h"

/* ***************************************************************** */

extern mcs_backend_t mcs_backend;

typedef struct {
	char *loc;
	GConfClient *client;
} mcs_gconf_handle_t;

mcs_handle_t *
mcs_gconf_new(char *domain)
{
	char scratch[PATH_MAX];

	mcs_gconf_handle_t *h = calloc(sizeof(mcs_gconf_handle_t), 1);
	mcs_handle_t *out = calloc(sizeof(mcs_handle_t), 1);

	g_type_init();

	out->base = &mcs_backend;
	out->mcs_priv_handle = h;

	snprintf(scratch, PATH_MAX, "/apps/%s", domain);

	h->loc = strdup(scratch);
	h->client = gconf_client_get_default();

	if (h->client == NULL)
		mcs_log("mcs_gconf_new(): Couldn't locate a GConf client handle to use.");

	return out;
}

void
mcs_gconf_destroy(mcs_handle_t *self)
{
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;

	g_object_unref(h->client);
	free(h->loc);
	free(h);

	free(self);
}

static char *
mcs_gconf_build_keypath(mcs_gconf_handle_t *self,
			const char *section, const char *key)
{
	static char out[PATH_MAX];

	if (section == NULL)
		snprintf(out, PATH_MAX, "%s/%s", self->loc, key);
	else
		snprintf(out, PATH_MAX, "%s/%s/%s", self->loc, section, key);

	return out;
}

static mcs_response_t
mcs_gconf_retrieve_value(mcs_gconf_handle_t *self,
			 const char *section, const char *key,
			 GConfValue **value)
{
	*value = gconf_client_get(self->client,
				  mcs_gconf_build_keypath(self, section, key),
				  NULL);

	return (*value != NULL) ? MCS_OK : MCS_FAIL;
}

static mcs_response_t
mcs_gconf_set_value(mcs_handle_t *h,
		    const char *section, const char *key,
		    GConfValue *value)
{
	mcs_gconf_handle_t *self = (mcs_gconf_handle_t *) h->mcs_priv_handle;

	gconf_client_set(self->client,
			 mcs_gconf_build_keypath(self, section, key),
			 value, NULL);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_get_string(mcs_handle_t *self, const char *section,
		       const char *key, char **value)
{
	GConfValue *val;
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;

	if (!mcs_gconf_retrieve_value(h, section, key, &val))
		return MCS_FAIL;
	if (val->type != GCONF_VALUE_STRING)
		return MCS_FAIL;

	*value = strdup(gconf_value_get_string(val));
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_get_int(mcs_handle_t *self, const char *section,
		    const char *key, int *value)
{
	GConfValue *val;
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;

	if (!mcs_gconf_retrieve_value(h, section, key, &val))
		return MCS_FAIL;
	if (val->type != GCONF_VALUE_INT)
		return MCS_FAIL;

	*value = gconf_value_get_int(val);
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_get_bool(mcs_handle_t *self, const char *section,
		     const char *key, int *value)
{
	GConfValue *val;
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;

	if (!mcs_gconf_retrieve_value(h, section, key, &val))
		return MCS_FAIL;
	if (val->type != GCONF_VALUE_BOOL)
		return MCS_FAIL;

	*value = gconf_value_get_bool(val);
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_get_float(mcs_handle_t *self, const char *section,
		      const char *key, float *value)
{
	GConfValue *val;
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;

	if (!mcs_gconf_retrieve_value(h, section, key, &val))
		return MCS_FAIL;
	if (val->type != GCONF_VALUE_FLOAT)
		return MCS_FAIL;

	*value = gconf_value_get_float(val);
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_get_double(mcs_handle_t *self, const char *section,
		       const char *key, double *value)
{
	GConfValue *val;
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;

	if (!mcs_gconf_retrieve_value(h, section, key, &val))
		return MCS_FAIL;
	if (val->type != GCONF_VALUE_FLOAT)
		return MCS_FAIL;

	*value = gconf_value_get_float(val);
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_set_string(mcs_handle_t *self, const char *section,
		       const char *key, const char *value)
{
	GConfValue *val;

	if (value == NULL)
		self->base->mcs_unset_key(self, section, key);
	else
	{
		val = gconf_value_new(GCONF_VALUE_STRING);
		gconf_value_set_string(val, value);
		mcs_gconf_set_value(self, section, key, val);
		gconf_value_free(val);
	}

	return MCS_OK;
}

mcs_response_t
mcs_gconf_set_int(mcs_handle_t *self, const char *section,
		    const char *key, int value)
{
	GConfValue *val;

	val = gconf_value_new(GCONF_VALUE_INT);
	gconf_value_set_int(val, value);
	mcs_gconf_set_value(self, section, key, val);
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_set_bool(mcs_handle_t *self, const char *section,
		     const char *key, int value)
{
	GConfValue *val;

	val = gconf_value_new(GCONF_VALUE_BOOL);
	gconf_value_set_bool(val, value);
	mcs_gconf_set_value(self, section, key, val);
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_set_float(mcs_handle_t *self, const char *section,
		      const char *key, float value)
{
	GConfValue *val;

	val = gconf_value_new(GCONF_VALUE_FLOAT);
	gconf_value_set_float(val, value);
	mcs_gconf_set_value(self, section, key, val);
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_set_double(mcs_handle_t *self, const char *section,
		       const char *key, double value)
{
	GConfValue *val;

	val = gconf_value_new(GCONF_VALUE_FLOAT);
	gconf_value_set_float(val, value);
	mcs_gconf_set_value(self, section, key, val);
	gconf_value_free(val);

	return MCS_OK;
}

mcs_response_t
mcs_gconf_unset_key(mcs_handle_t *self, const char *section,
		      const char *key)
{
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;

	gconf_client_unset(h->client, mcs_gconf_build_keypath(h, section, key), NULL);

	return MCS_OK;
}

mcs_list_t *
mcs_gconf_get_keys(mcs_handle_t *self, const char *section)
{
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;
	mcs_list_t *out = NULL;
	GSList *pairs;
	GError *error = NULL;

	pairs = gconf_client_all_entries(h->client, mcs_gconf_build_keypath(h, NULL, section), &error);

	for (; pairs != NULL; pairs = g_slist_next(pairs))
	{
		GConfEntry *val = (GConfEntry *) pairs->data;

		out = mcs_list_append(out, strdup(val->key));

		gconf_entry_free(val);
	}

	g_slist_free(pairs);

	return out;
}

mcs_list_t *
mcs_gconf_get_sections(mcs_handle_t *self)
{
	mcs_gconf_handle_t *h = (mcs_gconf_handle_t *) self->mcs_priv_handle;
	mcs_list_t *out = NULL;
	GSList *pairs;
	GError *error = NULL;

	pairs = gconf_client_all_dirs(h->client, h->loc, &error);

	for (; pairs != NULL; pairs = g_slist_next(pairs))
	{
		gchar *val = (gchar *) pairs->data;

		out = mcs_list_append(out, strdup(val));

		g_free(val);
	}

	g_slist_free(pairs);

	return out;
}

mcs_backend_t mcs_backend = {
	NULL,
	"gconf",
	mcs_gconf_new,
	mcs_gconf_destroy,

	mcs_gconf_get_string,
	mcs_gconf_get_int,
	mcs_gconf_get_bool,
	mcs_gconf_get_float,
	mcs_gconf_get_double,

	mcs_gconf_set_string,
	mcs_gconf_set_int,
	mcs_gconf_set_bool,
	mcs_gconf_set_float,
	mcs_gconf_set_double,

	mcs_gconf_unset_key,

	mcs_gconf_get_keys,
	mcs_gconf_get_sections
};
