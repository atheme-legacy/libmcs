/*
 * This is mcs; a modular configuration system.
 *
 * Copyright (c) 2009 Carlo Bramini <carlo.bramix -at- libero.it>
 *               2007 William Pitcock <nenolod -at- sacredspiral.co.uk>
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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <malloc.h>

#include "libmcs/mcs.h"

/**
 * \brief Loads all of the plugins in -DPLUGIN_DIR.
 */
void
mcs_load_plugins(void)
{
    HANDLE hFind;
    LPTSTR lpPattern;
    LPTSTR lpFileEnd;
    WIN32_FIND_DATA wfd;

    /* Allocate a buffer for building the TCHAR path */
    lpPattern = (LPTSTR)_alloca((sizeof(PLUGIN_DIR) + PATH_MAX)*sizeof(TCHAR));

    /* Point where PLUGIN_DIR finishes */
    lpFileEnd = lpPattern + sizeof(PLUGIN_DIR)-1;

    /* Copy the path into the destination */
    _tcscpy(lpPattern, _T(PLUGIN_DIR));

    /* Append '\\' if not included into PLUGIN_DIR */
    if (lpFileEnd[-1] != '\\')
        _tcscpy(lpFileEnd++, _T("\\"));

    /* Add the wildcards */
    _tcscpy(lpFileEnd, _T("*.*"));

    /* Start the search */
    hFind = FindFirstFile(lpPattern, &wfd);
    if (hFind == INVALID_HANDLE_VALUE)
        return;

    /* Remove the wildcards (no longer required) */
    lpFileEnd[0] = _T('\0');

    do
    {
        /* If it is a regula file... */
        if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            /* Check if it may be a plugin */
            if (_tcsstr(wfd.cFileName, _T(PLUGIN_SUFFIX)) != NULL)
            {
                mcs_backend_t *b;
                HMODULE        hDll;

                _tcscpy(lpFileEnd, wfd.cFileName);
                hDll = LoadLibrary(lpPattern);
                /* TODO: log */
                if (hDll != NULL)
                {
                    b = (mcs_backend_t *)GetProcAddress(hDll, "mcs_backend");
                    if (b != NULL)
                    {
                        b->handle = hDll;
                        mcs_backend_register(b);
                    }
                }
            }
        }
    } while (FindNextFile(hFind, &wfd));

    // Close the scan handle
    FindClose(hFind);
}

/**
 * \brief Unloads a given list of plugins.
 *
 * \param pl A mowgli.queue representing a list of plugins to unload.
 */
void
mcs_unload_plugins(mowgli_patricia_t *pl)
{
	mowgli_patricia_iteration_state_t state;
	mcs_backend_t *b;

	MOWGLI_PATRICIA_FOREACH(b, &state, pl)
	{
        FreeLibrary((HMODULE)(b->handle));
    }
}

