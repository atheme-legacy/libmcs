/*
 * This is mcs; a modular configuration system.
 *
 * Copyright (c) 2007-2011 William Pitcock <nenolod -at- sacredspiral.co.uk>
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

extern mcs_backend_t keyfile_backend; /* ../backends/default/keyfile.c */

/**
 * \brief A list of registered backends.
 *
 * This is a list of registered backends. It is not part of the public API.
 */
extern mowgli_patricia_t *mcs_backends;

/**
 * \brief Initialises the mcs library classes and loads the backend plugins.
 *
 * mcs_init() initialises mowgli.object, followed by all of the mcs
 * library classes which extend mowgli.object to provide an extensible
 * configuration management system.
 *
 * Once the library mowgli.object classes have been initialised, the mcs
 * plugin directory is iterated and various backend plugins are loaded.
 */
void
mcs_init(void)
{
	mowgli_init();

	mcs_backends = mowgli_patricia_create(mcs_strcasecanon);
	mcs_backend_register(&keyfile_backend);

	mcs_handle_class_init();
}

/**
 * \brief Releases resources used by the mcs backend plugins.
 *
 * This function unloads and releases resources used by the mcs backend
 * plugins.
 */
void
mcs_fini(void)
{
	mcs_backend_unregister(&keyfile_backend);
	mowgli_patricia_destroy(mcs_backends, NULL, NULL);
}

/**
 * \brief Retrieves the version of the mcs implementation.
 *
 * This function is useful for displaying the version of the system
 * implementation.
 *
 * \return A dynamically allocated string containing the mcs implementation.
 *         This should be used with free().
 */
char *
mcs_version(void)
{
	return strdup(PACKAGE_NAME "-" PACKAGE_VERSION);
}
