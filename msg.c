/*      $Id: msg.c,v 1.11 2005/01/31 13:42:34 se Exp $ */

/*
 * Copyright (c) 2004, 2005  Stephan Eckardt. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>

#include "msg.h"
#include "prsmsg.h"
#include "interface.h"

/* Some basic primitives */

struct slsk_str
mk_slskstr(char *str)
{
	struct slsk_str string;

	if (str != NULL) {
		string.len = strlen(str) + INTSIZE;
		string.c = str;
		return string;
	}
	else {
		string.len = 0;
		string.c = NULL;
		return string;
	}

}

struct slsk_buf *byte2msg (struct slsk_buf *msg, uint8_t b)
{
	memcpy (msg->buf + msg->pos, &b, sizeof(uint8_t));

	return NULL;
}

/* int2msg writes a 4 byte integer to pld and makes pld point
 * to the location after it. */
struct slsk_buf *int2msg (struct slsk_buf *pld, uint32_t i)
{
	memcpy (pld, &i, sizeof(uint32_t));

	return NULL;
}

/* str2msg writes a slsk-string to pld and makes pld point
 * to the location after it.
 *
 * A slsk-string consists of a 4 byte integer defining the length
 * and several chars. The string is not zero-terminated */
struct slsk_buf *str2msg(struct slsk_buf *msg, struct slsk_str str)
{
	int len;

	len = str.len - INTSIZE;

	int2msg (msg, len);
	memcpy (msg->buf, str.c, len);
//	msg->pos = msg->pos + len;

	return NULL;
}

/* np will point to the location after the byte */
SLSK_BYTE msg2byte (const struct slsk_buf *p, struct slsk_buf *np)
{
	SLSK_BYTE b;

	if (p->len - p->pos - 1 < BYTESIZE) {
		printf ("msg2byte: cannot read from buffer end!\n");
		return 0;
	} else {
		b = *((SLSK_BYTE *) p->buf + p->pos);
		if (np != NULL)
			np->pos = p->pos + BYTESIZE;
		return b;
	}
}

/* np will point to the location after the int */
SLSK_INT msg2int (const struct slsk_buf *p, struct slsk_buf *np)
{
	SLSK_INT i;

	if (p->len - p->pos - 1 < INTSIZE) {
		printf ("msg2int: cannot read from buffer end!\n");
		return 0;
	} else {
		i = *((SLSK_INT *) p->buf + p->pos);
		if (np != NULL)
			np->pos = p->pos + INTSIZE;
		return i;
	}
}

/* np will point to the location after the string */
char* msg2str (const struct slsk_buf *p, struct slsk_buf *np)
{
	SLSK_INT len = msg2int(p, NULL);
	char *str;

	if (len < (p->len - p->pos - INTSIZE)) {
		str = (char *) malloc(len + 1);
		memcpy (str, p->buf + p->pos + INTSIZE, len);
		str[len] = 0;

		if (np != NULL)
			np->pos = p->pos + INTSIZE + len;
		return str;
	} else {
		printf("msg2str: cannot read from buffer end!\n");
		return NULL;
	}
}

int getmsglen (const struct slsk_buf *msg)
{
	return msg2int(msg, NULL) + INTSIZE;
}

/* for debugging purposes */
int show_msg (const struct slsk_buf *msg)
{
	int i, len;

	len = msg2int(msg, NULL) + INTSIZE;	/* plus size of the length-field itself */

	for (i = 0; i < len; i++) {
		printf ("%c", msg->buf[i]);
	}
	return (0);
}

/* for protocol studying: writes a message with an unknown code to a file */
int dump_msg (const struct slsk_msg *msg)
{
	char *filename;
	struct slsk_buf nmsg;
	SLSK_INT msgcode;
	int fd;
	nmsg.buf = msg->buf;
	nmsg.len = msg->msglen;
	nmsg.pos = CODE_OFFSET;

	msgcode = msg2int(&nmsg, NULL);
	filename = malloc(100);

	snprintf(filename, 100, "dumps/%i-%s.dump", msgcode, getmsgname(msgcode));

	fd = open(filename, O_WRONLY | O_CREAT, S_IRWXU);
	write(fd, nmsg.buf, nmsg.len);
	close(fd);

	return 0;
}

