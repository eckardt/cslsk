/*      $Id: netio.h,v 1.11 2005/02/09 17:10:02 se Exp $ */

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


int conn_init (char* srvhost, uint16_t srvport);
struct slsk_peer *addpeer(char *hostname, int port, char *username, SLSK_BYTE nslots, SLSK_INT speed, SLSK_INT qlen, struct slsk_peers *);
struct slsk_conn *addconn(struct slsk_conns *conns);
int slsk_connect (struct slsk_conn *conn);
int conn_dstr (int sock);
int send_msg (struct slsk_conn *, struct slsk_msg *);
int getmsg(struct slsk_conn *conn, struct cslsk_conf *conf);
int putmsg(struct slsk_conn *conn, struct cslsk_conf *conf);
struct slsk_msg *postmkmsg (struct slsk_buf *);
int basic_msg2q (struct slsk_msg *smsg, struct slsk_msgs *msgq);
int msg2q (struct slsk_buf *buf, struct slsk_msgs *msgq);
