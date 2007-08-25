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

int
main(int argc, char *argv[])
{
	mcs_handle_t *h;
	mowgli_queue_t *groups, *i;
	int entries = 0, sections = 0;

	if (argc < 2)
	{
		printf("usage: %s domain\n", argv[0]);
		return -1;
	}

	mcs_init();

	h = mcs_new(argv[1]);
	groups = mcs_get_sections(h);

	for (i = groups; i != NULL; i = i->next)
	{
		mowgli_queue_t *keys, *i2;

		printf("\n%s\n", (char *) i->data);
		keys = mcs_get_keys(h, i->data);

		for (i2 = keys; i2 != NULL; i2 = i2->next)
		{
			printf("  %c-- %s\n", i2->next != NULL ? '|' : '`', (char *) i2->data);
			free(i2->data);

			entries++;
		}

		mowgli_queue_destroy(keys);
		free(i->data);

		sections++;
	}

	mowgli_queue_destroy(groups);
	mcs_fini();

	printf("\n%d entries across %d groups, average %d entries per group\n",
		entries, sections, entries / sections);

	return 0;
}
