/*      $Id: mksmsg.c,v 1.4 2005/02/09 17:10:01 se Exp $ */

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


#include <sys/types.h>
#include <strings.h>
#include <stdlib.h>
#include <stdarg.h>

#include "cslsk.h"
#include "msg.h"
#include "msg_codes.h"

#define BSIZE 1024

/*
 * mksmsg_* take specific arguments and create a message, that is to be sent
 * to the server.
 */
/*
char *
mk_smsghdr(SLSK_INT len, SLSK_INT code)
{
	char *msg;

	len += INTSIZE;	
//	 plus size of the length itself 
	if ((msg = (char *) malloc(len + INTSIZE)) != NULL) {
		int2msg(msg, len);
		int2msg(msg + CODE_OFFSET, code);
		return (msg);
	} else
		return NULL;
}*/

struct slsk_buf *
mksmsg(char *fmt, ...)
{
	va_list ap;
	SLSK_BYTE b = 0;
	SLSK_INT i = 0;
	SLSK_INT msglen = 0, slen = 0;
	SLSK_INT bneed = 0, loff = 0, l = 0;
	struct slsk_buf *buf;
	char *s;

	buf = malloc(sizeof(struct slsk_buf));
	buf->len = BSIZE;
	buf->pos = 0;
	buf->buf = malloc(buf->len);

	va_start(ap, fmt);
	while(*fmt)
		switch (*fmt++) {
			case 'l':
				l = 1;
				loff = buf->pos;
				bneed = INTSIZE;
				if ((int)(buf->len - buf->pos - 1 - bneed) < 0) { /* msg is not big enough*/
					buf->len += 1024*(bneed/1024) + BSIZE;
					buf->buf = realloc(buf->buf, buf->len);
				}
				buf->pos += INTSIZE;
				break;
			case 's':
				s = va_arg(ap, char *);
				slen = strlen(s);
				bneed = slen+INTSIZE;
				if ((int)(buf->len - buf->pos - 1 - bneed) < 0) { /* msg is not big enough*/
					buf->len += 1024*(bneed/1024) +  BSIZE;
					buf->buf = realloc(buf->buf, buf->len);
				}
				memcpy((buf->buf + buf->pos), &slen, INTSIZE);
				buf->pos += INTSIZE;
				memcpy((buf->buf + buf->pos), s, slen);
				buf->pos += slen;
				break;
			case 'b':
				b = va_arg(ap, int);
				bneed = BYTESIZE;
				if ((int)(buf->len - buf->pos - 1 - bneed) < 0) { /* msg is not big enough*/
					buf->len += 1024*(bneed/1024) +  BSIZE;
					buf->buf = realloc(buf->buf, buf->len);
				}
				memcpy((buf->buf + buf->pos), &b, BYTESIZE);
				buf->pos += BYTESIZE;
				break;
			case 'i':
				i = va_arg(ap, SLSK_INT);
				bneed = INTSIZE;
				if ((int)(buf->len - buf->pos - 1 - bneed) < 0) { /* msg is not big enough*/
					buf->len += 1024*(bneed/1024) +  BSIZE;
					buf->buf = realloc(buf->buf, buf->len);
				}
				memcpy((buf->buf + buf->pos), &i, INTSIZE);
				buf->pos += INTSIZE;
				break;
		}
	va_end(ap);
	msglen = buf->pos - loff - INTSIZE;;
	if (l)
		memcpy((buf->buf + loff), &msglen, INTSIZE);
	return buf;
}

