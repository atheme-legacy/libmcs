/*
 * This is mcs; a modular configuration system.
 *
 * Copyright (c) 2007 Diego Pettenò <flameeyes@gmail.com>
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

#include <kconfig.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <assert.h>

extern "C" {

  #include "libmcs/mcs.h"

/* ***************************************************************** */

  extern mcs_backend_t mcs_backend;
}

typedef struct {
	KConfig *cfg;
} mcs_kconfig_handle_t;

static mcs_handle_t *
mcs_kconfig_new(char *domain)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *)calloc(sizeof(mcs_kconfig_handle_t), 1);
	mcs_handle_t *out = (mcs_handle_t *)calloc(sizeof(mcs_handle_t), 1);

	if ( ! KApplication::kApplication() ) {
	  /* Initialise a fake KDE application if none present */

	  static char *fake_argv[] = { "mcs_backend" };

	  KCmdLineArgs::init(1, fake_argv, "mcs_backend", "MCS KConfig Backend", "Just a fake application to be able to use KConfig backend with non-KDE applications.", "9999", false);
	  new KApplication(false, false);
	}

	out->base = &mcs_backend;
	out->mcs_priv_handle = h;

	h->cfg = new KConfig(domain);

	return out;
}

static void
mcs_kconfig_destroy(mcs_handle_t *self)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	delete h->cfg;
	free(h);

	free(self);
}

static mcs_response_t
mcs_kconfig_get_string(mcs_handle_t *self, const char *section,
		       const char *key, char **value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	if ( ! h->cfg->hasGroup(section) )
	  return MCS_FAIL;

	h->cfg->setGroup(section);
	if ( ! h->cfg->hasKey(key) )
	  return MCS_FAIL;

	QString entry = h->cfg->readEntry(key);
	*value = strdup(entry.latin1());

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_get_int(mcs_handle_t *self, const char *section,
		    const char *key, int *value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	if ( ! h->cfg->hasGroup(section) )
	  return MCS_FAIL;

	h->cfg->setGroup(section);
	if ( ! h->cfg->hasKey(key) )
	  return MCS_FAIL;

	*value = h->cfg->readNumEntry(key);

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_get_bool(mcs_handle_t *self, const char *section,
		     const char *key, int *value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	if ( ! h->cfg->hasGroup(section) )
	  return MCS_FAIL;

	h->cfg->setGroup(section);
	if ( ! h->cfg->hasKey(key) )
	  return MCS_FAIL;

	*value = h->cfg->readBoolEntry(key);

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_get_float(mcs_handle_t *self, const char *section,
		      const char *key, float *value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	if ( ! h->cfg->hasGroup(section) )
	  return MCS_FAIL;

	h->cfg->setGroup(section);
	if ( ! h->cfg->hasKey(key) )
	  return MCS_FAIL;

	*value = (float)(h->cfg->readDoubleNumEntry(key));

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_get_double(mcs_handle_t *self, const char *section,
		       const char *key, double *value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	if ( ! h->cfg->hasGroup(section) )
	  return MCS_FAIL;

	h->cfg->setGroup(section);
	if ( ! h->cfg->hasKey(key) )
	  return MCS_FAIL;

	*value = h->cfg->readDoubleNumEntry(key);

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_set_string(mcs_handle_t *self, const char *section,
		       const char *key, const char *value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	h->cfg->setGroup(section);
	if ( ! value )
	  h->cfg->writeEntry(key, QString::null);
	else
	  h->cfg->writeEntry(key, value);

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_set_int(mcs_handle_t *self, const char *section,
		    const char *key, int value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	h->cfg->setGroup(section);
	h->cfg->writeEntry(key, value);

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_set_bool(mcs_handle_t *self, const char *section,
		     const char *key, int value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	h->cfg->setGroup(section);
	h->cfg->writeEntry(key, bool(value));

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_set_float(mcs_handle_t *self, const char *section,
		      const char *key, float value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	h->cfg->setGroup(section);
	h->cfg->writeEntry(key, value);

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_set_double(mcs_handle_t *self, const char *section,
		       const char *key, double value)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	h->cfg->setGroup(section);
	h->cfg->writeEntry(key, value);

	return MCS_OK;
}

static mcs_response_t
mcs_kconfig_unset_key(mcs_handle_t *self, const char *section,
		      const char *key)
{
	mcs_kconfig_handle_t *h = (mcs_kconfig_handle_t *) self->mcs_priv_handle;

	h->cfg->deleteEntry(section, key);

	return MCS_OK;
}

mcs_backend_t mcs_backend = {
	NULL,
	"kconfig",
	mcs_kconfig_new,
	mcs_kconfig_destroy,

	mcs_kconfig_get_string,
	mcs_kconfig_get_int,
	mcs_kconfig_get_bool,
	mcs_kconfig_get_float,
	mcs_kconfig_get_double,

	mcs_kconfig_set_string,
	mcs_kconfig_set_int,
	mcs_kconfig_set_bool,
	mcs_kconfig_set_float,
	mcs_kconfig_set_double,

	mcs_kconfig_unset_key
};
