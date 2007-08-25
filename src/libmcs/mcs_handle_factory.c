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

mowgli_queue_t *mcs_backends_lst = NULL;

/* ******************************************************************* */

static mowgli_object_class_t klass;

static void
mcs_handle_destroy(mcs_handle_t *self)
{
	self->base->mcs_destroy(self);
}

/**
 * \brief Initialises the mowgli.object.class for the mcs.handle object.
 */
void
mcs_handle_class_init(void)
{
	mowgli_object_class_init(&klass, "mcs.handle", mcs_handle_destroy, FALSE);
}

/* ******************************************************************* */

mcs_handle_t *
mcs_new(char *domain)
{
	mowgli_queue_t *n;
	char *magic = mcs_backend_select();

	if (magic == NULL)
		magic = "default";

	for (n = mcs_backends_lst; n != NULL; n = n->next)
	{
		mcs_backend_t *b = (mcs_backend_t *) n->data;

		if (!strcmp(b->name, magic))
		{
			mcs_handle_t *out = b->mcs_new(domain);
			mowgli_object_init(mowgli_object(out), NULL, &klass, NULL);

			return out;
		}
	}

	return NULL;
}

/**
 * \brief Destroys an mcs.handle object.
 *
 * This function is considered deprecated since version 0.6. It should 
 * not be used in any newly written code.
 */
void
mcs_destroy(mcs_handle_t *self)
{
	mowgli_object_unref(self);
}

/* ******************************************************************* */

/**
 * \brief Public function to retrieve a string value from a configuration database.
 *
 * This function is the public interface for retrieving a string value from
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to look in.
 * \param key The key to look up.
 * \param value A memory location to put the value in.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_get_string(mcs_handle_t *self,
	       const char *section,
	       const char *key,
	       char **value)
{
	return self->base->mcs_get_string(self, section, key, value);
}

/**
 * \brief Public function to retrieve an integer value from a 
 *        configuration database.
 *
 * This function is the public interface for retrieving an integer value from
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to look in.
 * \param key The key to look up.
 * \param value A memory location to put the value in.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_get_int(mcs_handle_t *self,
	    const char *section,
	    const char *key,
	    int *value)
{
	return self->base->mcs_get_int(self, section, key, value);
}

/**
 * \brief Public function to retrieve a boolean value from a 
 *        configuration database.
 *
 * This function is the public interface for retrieving a boolean value from
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to look in.
 * \param key The key to look up.
 * \param value A memory location to put the value in.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_get_bool(mcs_handle_t *self,
	     const char *section,
	     const char *key,
	     int *value)
{
	return self->base->mcs_get_bool(self, section, key, value);
}

/**
 * \brief Public function to retrieve a floating point value from a 
 *        configuration database.
 *
 * This function is the public interface for retrieving a float value from
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to look in.
 * \param key The key to look up.
 * \param value A memory location to put the value in.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_get_float(mcs_handle_t *self,
	      const char *section,
	      const char *key,
	      float *value)
{
	return self->base->mcs_get_float(self, section, key, value);
}

/**
 * \brief Public function to retrieve a double-precision floating point 
 *        value from a configuration database.
 *
 * This function is the public interface for retrieving a double value from
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to look in.
 * \param key The key to look up.
 * \param value A memory location to put the value in.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_get_double(mcs_handle_t *self,
	       const char *section,
	       const char *key,
	       double *value)
{
	return self->base->mcs_get_double(self, section, key, value);
}

/* ******************************************************************* */

/**
 * \brief Public function to set a string value in a configuration database.
 *
 * This function is the public interface for setting a string value in
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to set the value in.
 * \param key The key to set.
 * \param value The value to set.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_set_string(mcs_handle_t *self,
	       const char *section,
	       const char *key,
	       const char *value)
{
	return self->base->mcs_set_string(self, section, key, value);
}

/**
 * \brief Public function to set an integer value in a configuration database.
 *
 * This function is the public interface for setting an integer value in
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to set the value in.
 * \param key The key to set.
 * \param value The value to set.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_set_int(mcs_handle_t *self,
	    const char *section,
	    const char *key,
	    int value)
{
	return self->base->mcs_set_int(self, section, key, value);
}

/**
 * \brief Public function to set a boolean value in a configuration database.
 *
 * This function is the public interface for boolean a string value in
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to set the value in.
 * \param key The key to set.
 * \param value The value to set.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_set_bool(mcs_handle_t *self,
	     const char *section,
	     const char *key,
	     int value)
{
	return self->base->mcs_set_bool(self, section, key, value);
}

/**
 * \brief Public function to set a floating point value in a configuration database.
 *
 * This function is the public interface for setting a float value in
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to set the value in.
 * \param key The key to set.
 * \param value The value to set.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_set_float(mcs_handle_t *self,
	      const char *section,
	      const char *key,
	      float value)
{
	return self->base->mcs_set_float(self, section, key, value);
}

/**
 * \brief Public function to set a double-precision floating point value
 *        in a configuration database.
 *
 * This function is the public interface for setting a double value in
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to set the value in.
 * \param key The key to set.
 * \param value The value to set.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_set_double(mcs_handle_t *self,
	       const char *section,
	       const char *key,
	       double value)
{
	return self->base->mcs_set_double(self, section, key, value);
}

/* ******************************************************************* */

/**
 * \brief Public function to remove a value from a configuration database.
 *
 * This function is the public interface for removing a value from
 * a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to unset the value in.
 * \param key The key to unset.
 *
 * \return A mcs_response_t value representing the success or failure of 
 *         the transaction.
 */
mcs_response_t
mcs_unset_key(mcs_handle_t *self,
	      const char *section,
	      const char *key)
{
	return self->base->mcs_unset_key(self, section, key);
}

/* ******************************************************************* */

/**
 * \brief Public function to retrieve a mowgli.queue of keys from a
 *        configuration database.
 *
 * This function is the public interface for retrieving a mowgli.queue of
 * keys from a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 * \param section The section to search.
 *
 * \return A mowgli.queue of key names in the requested section.
 */
mowgli_queue_t *
mcs_get_keys(mcs_handle_t *self,
	     const char *section)
{
	return self->base->mcs_get_keys(self, section);
}

/* ******************************************************************* */

/**
 * \brief Public function to retrieve a mowgli.queue of sections from a
 *        configuration database.
 *
 * This function is the public interface for retrieving a mowgli.queue of
 * sections from a configuration database via a configuration backend class.
 *
 * \param self The mcs.handle object that represents the configuration database.
 *
 * \return A mowgli.queue of section names in the requested section.
 */
mowgli_queue_t *
mcs_get_sections(mcs_handle_t *self)
{
	return self->base->mcs_get_sections(self);
}
