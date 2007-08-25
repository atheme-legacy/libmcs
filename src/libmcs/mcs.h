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

#ifndef __LIBMCS_MCS_H__
#define __LIBMCS_MCS_H__

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <errno.h>

#include <mowgli.h>

#ifdef _MCS_CORE
# include <libmcs/mcs_config.h>
#endif

/*! mcs_response_t denotes the status of a transaction. */
typedef enum {
	MCS_FAIL, /*!< designates transaction failure */
	MCS_OK    /*!< designates transaction success */
} mcs_response_t;

/** Friendly name for struct mcs_handle_ */
typedef struct mcs_handle_ mcs_handle_t;

/**
 * \brief Contains the vtable and some references for an mcs storage backend.
 *
 * Storage backends are provided by modules which are loaded during 
 * mcs_init().
 *
 * Your typical storage backend will include at least these functions,
 * although the backend interface may and likely will change in mcs2.
 * That is being considered for later, though, so you should write 
 * backends compliant to this API for now.
 *
 * For some example backends, look in the MCS source.
 */
typedef struct {
	void *handle; /*!< dlopen(3) handle, filled in by mcs. */

	/**
	 * \brief The unique name identifying the backend.
	 *
	 * This is the name of the backend, e.g. KDE's KConfig storage
	 * system would be called "kconfig". It is used to uniquely 
	 * identify the storage backend so that it can be selected in
	 * via policies and programatically.
	 *
	 * Custom backends should use an application-specific name unless
	 * they have global scope. In any case, the name should be unique.
	 */
	const char *name;

	/* constructors and destructors */

	/**
	 * \brief Creates a new mcs.handle object.
	 *
 	 * The mcs.handle object returned by this function is not fully 
	 * initialized in this function, base mowgli.object work is done
	 * in the primary constructor. This just creates the derived
	 * class which is presented to the user as mcs.handle.
	 *
	 * As such, you should avoid mowgli.object functions in both
	 * the constructor and destructor vtable functions.
	 *
	 * \param domain A string representing the domain to associate with.
	 */
	mcs_handle_t *(*mcs_new)(char *domain);

	/**
	 * \brief Implementation-specific destructor function.
	 *
	 * This is the destructor for your mcs.handle sub-class.
	 * Base mowgli.object work has already been done by the time this
	 * is called, therefore mowgli.object work should be avoided here.
	 *
	 * \param handle A mcs.handle object to deinitialise.
	 */
	void (*mcs_destroy)(mcs_handle_t *handle);

	/* retrieval */

	/**
	 * \brief Retrieves a string from the configuration backend.
	 *
	 * \param handle A mcs.handle object to search for the key in.
	 * \param section A section name to look for the key in.
	 * \param key The name of the key to look up.
	 * \param value A pointer to the memory location to put the value in.
	 */
	mcs_response_t (*mcs_get_string)(mcs_handle_t *handle,
					 const char *section,
					 const char *key,
					 char **value);

	/**
	 * \brief Retrieves an integer from the configuration backend.
	 *
	 * \param handle A mcs.handle object to search for the key in.
	 * \param section A section name to look for the key in.
	 * \param key The name of the key to look up.
	 * \param value A pointer to the memory location to put the value in.
	 */
	mcs_response_t (*mcs_get_int)(mcs_handle_t *handle,
				      const char *section,
				      const char *key,
				      int *value);

	/**
	 * \brief Retrieves a boolean value from the configuration backend.
	 *
	 * \param handle A mcs.handle object to search for the key in.
	 * \param section A section name to look for the key in.
	 * \param key The name of the key to look up.
	 * \param value A pointer to the memory location to put the value in.
	 */
	mcs_response_t (*mcs_get_bool)(mcs_handle_t *handle,
				       const char *section,
				       const char *key,
				       int *value);

	/**
	 * \brief Retrieves a float value from the configuration backend.
	 *
	 * \param handle A mcs.handle object to search for the key in.
	 * \param section A section name to look for the key in.
	 * \param key The name of the key to look up.
	 * \param value A pointer to the memory location to put the value in.
	 */
	mcs_response_t (*mcs_get_float)(mcs_handle_t *handle,
				        const char *section,
				        const char *key,
				        float *value);

	/**
	 * \brief Retrieves a double-precision float value from the configuration backend.
	 *
	 * \param handle A mcs.handle object to search for the key in.
	 * \param section A section name to look for the key in.
	 * \param key The name of the key to look up.
	 * \param value A pointer to the memory location to put the value in.
	 */
	mcs_response_t (*mcs_get_double)(mcs_handle_t *handle,
				         const char *section,
				         const char *key,
				         double *value);

	/* setting data */

	/**
	 * \brief Sets a string value in the configuration backend.
	 *
	 * \param handle A mcs.handle object to add the key to.
	 * \param section A section name to add the key to.
	 * \param key The name of the key to add.
	 * \param value The value the key should have.
	 */
	mcs_response_t (*mcs_set_string)(mcs_handle_t *handle,
					 const char *section,
					 const char *key,
					 const char *value);

	/**
	 * \brief Sets an integer value in the configuration backend.
	 *
	 * \param handle A mcs.handle object to add the key to.
	 * \param section A section name to add the key to.
	 * \param key The name of the key to add.
	 * \param value The value the key should have.
	 */
	mcs_response_t (*mcs_set_int)(mcs_handle_t *handle,
				      const char *section,
				      const char *key,
				      int value);

	/**
	 * \brief Sets a boolean value in the configuration backend.
	 *
	 * \param handle A mcs.handle object to add the key to.
	 * \param section A section name to add the key to.
	 * \param key The name of the key to add.
	 * \param value The value the key should have.
	 */
	mcs_response_t (*mcs_set_bool)(mcs_handle_t *handle,
				       const char *section,
				       const char *key,
				       int value);

	/**
	 * \brief Sets a floating point value in the configuration backend.
	 *
	 * \param handle A mcs.handle object to add the key to.
	 * \param section A section name to add the key to.
	 * \param key The name of the key to add.
	 * \param value The value the key should have.
	 */
	mcs_response_t (*mcs_set_float)(mcs_handle_t *handle,
				        const char *section,
				        const char *key,
				        float value);

	/**
	 * \brief Sets a double-precision floating point value in the configuration backend.
	 *
	 * \param handle A mcs.handle object to add the key to.
	 * \param section A section name to add the key to.
	 * \param key The name of the key to add.
	 * \param value The value the key should have.
	 */
	mcs_response_t (*mcs_set_double)(mcs_handle_t *handle,
				         const char *section,
				         const char *key,
				         double value);

	/* unset */

	/**
	 * \brief Removes a value from the configuration backend.
	 *
	 * \param handle A mcs.handle object to remove the key from.
	 * \param section A section name to remove the key from.
	 * \param key The name of the key to remove.
	 */
	mcs_response_t (*mcs_unset_key)(mcs_handle_t *handle,
					const char *section,
					const char *key);

	/* key request */

	/**
	 * \brief Returns a deque of keys associated with a section.
	 *
	 * \param handle A mcs.handle object to get the keys from.
	 * \param section A section name to get the keys from.
	 */
	mowgli_queue_t *(*mcs_get_keys)(mcs_handle_t *handle,
				    const char *section);

	/* sections request */

	/**
	 * \brief Returns a deque of sections associated with the config backend.
	 *
	 * \param handle A mcs.handle object to get the sections from.
	 */
	mowgli_queue_t *(*mcs_get_sections)(mcs_handle_t *handle);
} mcs_backend_t;

/**
 * \brief Represents an MCS object handle.
 */
struct mcs_handle_ {
	mowgli_object_t object;  /*!< mowgli.object parent. */
	mcs_backend_t *base;     /*!< vtable of backend functions */
	void *mcs_priv_handle;   /*!< backend-specific opaque data */
};

/*
 * These functions have to do with initialization of the
 * library.
 */

extern void  mcs_init(void);
extern void  mcs_fini(void);
extern char *mcs_version(void);
extern void  mcs_handle_class_init(void);
 
/*
 * These functions have to do with registration of MCS backends.
 */
extern mcs_response_t mcs_backend_register(mcs_backend_t *backend);
extern mcs_response_t mcs_backend_unregister(mcs_backend_t *backend);
extern mowgli_queue_t *   mcs_backend_get_list(void);
extern char *         mcs_backend_select(void);

/*
 * These functions provide the public interface for creating and closing MCS
 * handles.
 *
 * Please note that if a handle is not closed, the data may not be saved to
 * disk.
 */
extern mcs_handle_t *mcs_new(char *domain);
extern void mcs_destroy(mcs_handle_t *handle);

/*
 * These functions provide the public interface for querying and setting data.
 */
/* retrieval */
extern mcs_response_t mcs_get_string(mcs_handle_t *handle,
				 const char *section,
				 const char *key,
				 char **value);

extern mcs_response_t mcs_get_int(mcs_handle_t *handle,
			      const char *section,
			      const char *key,
			      int *value);

extern mcs_response_t mcs_get_bool(mcs_handle_t *handle,
			       const char *section,
			       const char *key,
			       int *value);

extern mcs_response_t mcs_get_float(mcs_handle_t *handle,
			        const char *section,
			        const char *key,
			        float *value);

extern mcs_response_t mcs_get_double(mcs_handle_t *handle,
			         const char *section,
			         const char *key,
			         double *value);

/* setting data */
extern mcs_response_t mcs_set_string(mcs_handle_t *handle,
				 const char *section,
				 const char *key,
				 const char *value);

extern mcs_response_t mcs_set_int(mcs_handle_t *handle,
			      const char *section,
			      const char *key,
			      int value);

extern mcs_response_t mcs_set_bool(mcs_handle_t *handle,
			       const char *section,
			       const char *key,
			       int value);

extern mcs_response_t mcs_set_float(mcs_handle_t *handle,
			        const char *section,
			        const char *key,
			        float value);

extern mcs_response_t mcs_set_double(mcs_handle_t *handle,
			         const char *section,
			         const char *key,
			         double value);

/* unset */
extern mcs_response_t mcs_unset_key(mcs_handle_t *handle,
				const char *section,
				const char *key);

/* key request */
extern mowgli_queue_t *mcs_get_keys(mcs_handle_t *handle,
				const char *section);

extern mowgli_queue_t *mcs_get_sections(mcs_handle_t *handle);

/*
 * These functions have to do with the plugin loader.
 */
extern void mcs_load_plugins(void);
extern void mcs_unload_plugins(mowgli_queue_t *l);

/*
 * These functions are utility functions.
 */
extern size_t mcs_strnlen(const char *str, size_t len);
extern char * mcs_strndup(const char *str, size_t len);
extern int mcs_create_directory(const char *path, mode_t mode);
extern size_t mcs_strlcat(char *dest, const char *src, size_t count);
extern size_t mcs_strlcpy(char *dest, const char *src, size_t count);

#endif
