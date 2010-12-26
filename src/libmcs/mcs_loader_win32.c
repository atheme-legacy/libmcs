/*
 * This is mcs; a modular configuration system.
 *
 * Copyright (c) 2010 John Lindgren <john.lindgren -at- tds.net>
 *               2009 Carlo Bramini <carlo.bramix -at- libero.it>
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

static void normalize_path(TCHAR *path)
{
    TCHAR *c;
    for (c = path; *c; c++)
    {
        if (*c == '/')
            *c = '\\';
    }
    c--;
    if (*c == '\\' && c > path + 2)
        *c = 0;
}

static TCHAR *last_path_element(TCHAR *path)
{
    TCHAR *c = _tcsrchr(path, '\\');
    return (c && c[1]) ? c + 1 : NULL;
}

static void strip_path_element(TCHAR *path, TCHAR *elem)
{
    if (elem > path + 3)
        elem[-1] = 0;
    else
        *elem = 0;
}

static const TCHAR *get_plugin_dir(void)
{
    static char called = 0, failed = 0;
    static TCHAR dir[MAX_PATH];

    if (called)
        return failed ? _T(PLUGIN_DIR) : dir;
    called = 1;

    /* The folder where libmcs.dll was installed. */
    TCHAR old[] = _T(BIN_DIR);
    normalize_path(old);

    /* Where is it now? */
    MEMORY_BASIC_INFORMATION m;
    if (!VirtualQuery(get_plugin_dir, &m, sizeof m))
        goto ERR;
    TCHAR new[MAX_PATH];
    int len = GetModuleFileName(m.AllocationBase, new, MAX_PATH);
    if (!len || len == MAX_PATH)
        goto ERR;
    normalize_path(new);

    /* Strip the base name. */
    TCHAR *base = last_path_element(new);
    if (!base)
        goto ERR;
    strip_path_element(new, base);

    /* Strip innermost folder names from both paths as long as they match.  This
     * leaves a compile-time prefix and a run-time one to replace it with. */
    TCHAR *a, *b;
    while ((a = last_path_element(old)) && (b = last_path_element(new)) && !_tcsicmp(a, b))
    {
        strip_path_element(old, a);
        strip_path_element(new, b);
    }

    /* The folder where plugins were installed. */
    TCHAR olddir[] = _T(PLUGIN_DIR);
    normalize_path(olddir);

    /* Where we expect them to be now. */
    len = _tcslen(old);
    if (_tcsnicmp(olddir, old, len) || _tcslen(new) + _tcslen(olddir + len) + 1 > MAX_PATH)
        goto ERR;
    _tcscpy(dir, new);
    _tcscpy(dir + _tcslen(dir), olddir + len);
    return dir;

ERR:
    failed = 1;
    return _T(PLUGIN_DIR);
}

/**
 * \brief Loads all of the plugins in <dllfolder>\mcs, where <dllfolder> is the
 * folder containing libmcs.dll.
 */
void
mcs_load_plugins(void)
{
    HANDLE hFind;
    LPTSTR lpPattern;
    LPTSTR lpFileEnd;
    WIN32_FIND_DATA wfd;

    /* Allocate a buffer for building the TCHAR path */
    int len = _tcslen(get_plugin_dir());
    lpPattern = (LPTSTR)_alloca((len + 1 + PATH_MAX)*sizeof(TCHAR));

    /* Point where PLUGIN_DIR finishes */
    lpFileEnd = lpPattern + len;

    /* Copy the path into the destination */
    _tcscpy(lpPattern, get_plugin_dir());

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

