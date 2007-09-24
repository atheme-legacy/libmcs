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

extern mowgli_queue_t *mcs_backends_lst;

/**
 * \brief Registers a custom mcs.backend.
 *
 * This is used to register a named mcs.backend vtable with mcs.
 * This should be unregistered with mcs_backend_unregister() when it is
 * no longer needed, such as during an application unload.
 *
 * \param b An mcs.backend vtable to register.
 * \return A mcs_response_t code depending on success or failure of the
 *         registration.
 */
mcs_response_t
mcs_backend_register(mcs_backend_t *b)
{
	mcs_backends_lst = mowgli_queue_shift(mcs_backends_lst, b);

	return MCS_OK;
}

/**
 * \brief Unregisters a custom mcs.backend.
 *
 * This is used to unregister a named mcs.backend vtable with mcs.
 * This should be called when you are no longer interested in using the
 * backend implementation with mcs.
 *
 * \param b An mcs.backend vtable to register.
 * \return A mcs_response_t code depending on success or failure of the
 *         deregistration.
 */
mcs_response_t
mcs_backend_unregister(mcs_backend_t *b)
{
	mcs_backends_lst = mowgli_queue_remove_data(mcs_backends_lst, b);

	return MCS_OK;
}

/**
 * \brief Returns a mowgli.queue representing the backends list.
 *
 * This is the portable version of using the mcs_backends_lst symbol.
 *
 * \return A mowgli.queue representing the backend list.
 */
mowgli_queue_t *
mcs_backend_get_list(void)
{
	return mcs_backends_lst;
}

/**
 * \brief Determines the backend that should be used.
 *
 * This function checks the environment variable, MCS_BACKEND first. If 
 * that is empty, it checks $HOME/.mcs-backend, followed by 
 * /etc/mcs-backend. If no preference is found, it returns "default".
 *
 * \return The name of the backend that should be used.
 */
const char *
mcs_backend_select(void)
{
	char *env = getenv("MCS_BACKEND");
	char buf[1024];
	FILE *f;
	mowgli_queue_t *l;
	char *tptr;

	if (env != NULL)
		return env;

	env = getenv("HOME");

	if (env == NULL)
		return NULL;

	snprintf(buf, 1024, "%s/.mcs-backend", env);

	f = fopen(buf, "rb");

	if (f != NULL)
	{
		fgets(buf, 1024, f);
		fclose(f);
	}
	else
	{
		f = fopen(MCS_SYSCONFDIR "/mcs-backend", "rb");

		if (f != NULL)
		{
			fgets(buf, 1024, f);
			fclose(f);
		}
	}

	if ((tptr = strchr(buf, '\n')) != NULL)
		*tptr = '\0';
	else
		return "default";

	/* check to make sure we have this backend */
	for (l = mcs_backend_get_list(); l != NULL; l = l->next)
	{
		mcs_backend_t *b = (mcs_backend_t *) l->data;

		if (!strcasecmp(b->name, buf))
			return b->name;
	}

	return "default";
}
