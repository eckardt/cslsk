/*      $Id: interface.h,v 1.8 2005/02/09 17:10:01 se Exp $ */

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


#include "cslsk.h"
void eval_input (char* command, struct cslsk_conf *);
void do_login (char *argv[], struct cslsk_conf *, struct slsk_msgs *msgs);
void do_logout (char *argv[], struct cslsk_conf *);
void do_updateroomlist (char *argv[], struct cslsk_conf *, struct slsk_msgs *msgs);
void do_roomlist (char *argv[], struct slsk_roomlist *);
void do_joinroom(char *argv[], struct cslsk_conf *, struct slsk_msgs *msgs);
void do_leaveroom(char *argv[], struct cslsk_conf *, struct slsk_msgs *msgs);
void do_saychatroom (char *argv[], struct slsk_msgs *msgs);
void do_status (char *argv[], struct cslsk_conf *);
void do_exit (char *argv[]);
void do_peerconn (char *argv[], struct cslsk_conf *);
void do_search (char *argv[], struct slsk_msgs *msgs, struct slsk_searches *);
void do_listsearches (char *argv[], struct slsk_searches *);
void do_listtransfers (char *argv[], struct slsk_transfers *transfers);
void do_download(char *argv[], struct cslsk_conf *conf, struct slsk_searches *searches);
void do_getfilelist(char *argv[], struct cslsk_conf *conf);
void do_showfilelist(char *argv[], struct cslsk_conf *conf);
char* getmsgname (uint32_t code);
char* getpmsgname (uint32_t code);
