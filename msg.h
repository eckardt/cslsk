/*      $Id: msg.h,v 1.9 2005/01/31 09:26:03 se Exp $ */

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


#ifndef __CSLSK_MSG_H
#define __CSLSK_MSG_H

#include <sys/types.h>
#include <stdint.h>

#include "cslsk.h"

/*
 * A Slskmsg looks like this:
 *  ____________________________________________________________________________
 * | msg_len (uint32) | msg_code (uint32) | msg_pld (dependant on the msg_code) |
 *  ----------------------------------------------------------------------------
 */

#define PLD_OFFSET	8
#define CODE_OFFSET	4
#define INTSIZE	sizeof(uint32_t)
#define BYTESIZE  sizeof(uint8_t)

struct slsk_str mk_slskstr (char *);
struct slsk_buf *str2msg (struct slsk_buf *, struct slsk_str);
struct slsk_buf *int2msg (struct slsk_buf *, uint32_t);
struct slsk_buf *byte2msg (struct slsk_buf *, uint8_t);
//SLSK_BYTE msg2byte (const char* pld, char **np);
//SLSK_INT msg2int (const char* pld, char **np);
//char* msg2str (const char* pld, char **np);
SLSK_BYTE msg2byte (const struct slsk_buf *p, struct slsk_buf *np);
SLSK_INT msg2int (const struct slsk_buf *p, struct slsk_buf *np);
char* msg2str (const struct slsk_buf *p, struct slsk_buf *np);


char* mk_hdr (char*, char*, uint32_t);

int getmsglen (const struct slsk_buf *msg);
int show_msg (const struct slsk_buf *msg);
int dump_msg (const struct slsk_msg *msg);

#endif /* __CSLSK_MSG_H */
