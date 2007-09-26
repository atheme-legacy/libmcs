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

/**
 * \brief Determines the length of a string, limited by an arbitrary length.
 *
 * This is a function which is useful for limiting the length of a string.
 *
 * \param str The string to test.
 * \param len The maximum allowed length of the string.
 * \return The length of the string or len, whichever is smaller.
 */
size_t
mcs_strnlen(const char *str, size_t len)
{
	const char *end = memchr(str, '\0', len);

	return end != NULL ? (size_t) (end - str) : len;
}

/**
 * \brief Duplicates a string, limited to a specific length.
 *
 * This is a function which duplicates a string limited to a maximum length.
 *
 * \param str The string to duplicate.
 * \param len The maximum allowed length of the string.
 * \return The length of the string or len, whichever is smaller.
 */
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

/**
 * \brief A utility function which creates a directory.
 *
 * This function creates a directory. It recurses to handle any parents
 * which also need to be created.
 *
 * \param path The path to create.
 * \param mode The mode of the directory that should be created.
 * \return 0 on success, -1 on failure.
 */
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

#ifdef __MINGW32__
		if (mkdir(ttptr) == -1 && errno != EEXIST)
#else
		if (mkdir(ttptr, mode) == -1 && errno != EEXIST)
#endif
		{
			mowgli_log("mcs_create_directory(): mkdir '%s': %s",
				ttptr, strerror(errno));
			return -1;
		}

		free(ttptr);
	}

#ifdef __MINGW32__
	if (mkdir(pptr) == -1 && errno != EEXIST)
#else
	if (mkdir(pptr, mode) == -1 && errno != EEXIST)
#endif
	{
		mowgli_log("mcs_create_directory(): mkdir '%s': %s",
			pptr, strerror(errno));
		return -1;
	}

	free(pptr);

	return 0;
}

/**
 * \brief Concatenates a string, limited to a maximum buffer size.
 *
 * This is a function which is useful for safely copying a string.
 *
 * \param dest The string to copy to.
 * \param src The string to copy from.
 * \param count The maximum allowed length of the string.
 * \return The length of the new string.
 */
size_t mcs_strlcat(char *dest, const char *src, size_t count)
{
	size_t dsize = strlen(dest);
	size_t len = strlen(src);
	size_t res = dsize + len;

	dest += dsize;
	count -= dsize;

	if (len >= count)
		len = count - 1;

	memcpy(dest, src, len);
	dest[len] = 0;

	return res;
}

/**
 * \brief Copies a string, limited to a maximum buffer size.
 *
 * This is a function which is useful for safely copying a string.
 *
 * \param dest The string to copy to.
 * \param src The string to copy from.
 * \param size The maximum allowed length of the string.
 * \return The length of the new string.
 */
size_t mcs_strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size != 0)
	{
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}

	return ret;
}

