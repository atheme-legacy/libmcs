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

size_t
mcs_strnlen(const char *str, size_t len)
{
	const char *end = memchr(str, '\0', len);

	return end != NULL ? (size_t) (end - str) : len;
}

char *
mcs_strndup(const char *str, size_t len)
{
	size_t nlen = mcs_strnlen(str, len);
	char *out = malloc(nlen + 1);

	if (out == NULL)
		return NULL;

	memcpy(out, str, nlen);
	out[len] = '\0';

	return out;
}

int
mcs_create_directory(const char *path, mode_t mode)
{
	char *pptr, *tptr;

	if (path == NULL || *path == '\0')
	{
		errno = EINVAL;
		return -1;
	}

	pptr = strdup(path);

	/* make the structure before this directory */
	for (tptr = strchr(pptr + 1, '/'); tptr != NULL && *tptr != '\0';
		tptr = strchr(tptr + 1, '/'))
	{
		char *ttptr = mcs_strndup(pptr, (size_t) (tptr - pptr));

		if (mkdir(ttptr, mode) == -1 && errno != EEXIST)
		{
			mcs_log("mcs_create_directory(): mkdir '%s': %s",
				ttptr, strerror(errno));
			return -1;
		}

		free(ttptr);
	}

	if (mkdir(pptr, mode) == -1 && errno != EEXIST)
	{
		mcs_log("mcs_create_directory(): mkdir '%s': %s",
			pptr, strerror(errno));
		return -1;
	}

	free(pptr);

	return 0;
}
