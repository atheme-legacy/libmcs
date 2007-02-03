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

/*
 * mcs_load_plugins
 *
 * Loads all of the plugins in -DPLUGIN_DIR.
 */
void
mcs_load_plugins(void)
{
	DIR *pl_dir = NULL;
	struct dirent *ldirent = NULL;
	char fn[PATH_MAX];

	pl_dir = opendir(PLUGIN_DIR);

	if (pl_dir == NULL)
		return;

	while ((ldirent = readdir(pl_dir)) != NULL)
	{
		void *dl_handle;
		mcs_backend_t *b;

		if (!strstr(ldirent->d_name, ".so"))
			continue;

		snprintf(fn, sizeof(fn), "%s/%s", PLUGIN_DIR, ldirent->d_name);

		dl_handle = dlopen(fn, RTLD_LAZY);

		/* TODO: log */
		if (dl_handle == NULL)
			continue;

		b = dlsym(dl_handle, "mcs_backend");

		if (b == NULL)
		{
			dlclose(dl_handle);
			continue;
		}

		b->handle = dl_handle;

		mcs_backend_register(b);
	}

        closedir(pl_dir);
}

/*
 * mcs_close_plugins
 *
 * Unloads a given list of plugins.
 */
void
mcs_unload_plugins(mcs_list_t *pl)
{
	mcs_list_t *n;

	for (n = pl; n != NULL; n = n->next)
	{
		mcs_backend_t *b = (mcs_backend_t *) n->data;
		void *handle = b->handle;

		dlclose(handle);
	}
}

