/*      $Id: cslsk.h,v 1.10 2005/02/09 17:10:01 se Exp $ */

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


#ifndef __CSLSK_CSLSK_H
#define __CSLSK_CSLSK_H

#include "compat/queue.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <netinet/in.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

typedef uint8_t		 SLSK_BYTE;
typedef uint32_t	 SLSK_INT;

#define	SLSK_PSTAT_CONNECTED	1
#define SLSK_TSTAT_FAILED	1

#define SLSK_CONST_UPLOAD	1
#define SLSK_CONST_DOWNLOAD	0
#define SLSK_CONST_UNKNOWN	-1
#define SLSK_CONST_OFFLINE	0
#define SLSK_CONST_AWAY		1
#define SLSK_CONST_ONLINE	2

struct slsk_str {			/* this string is not zero-terminated */
	SLSK_INT	 len;		/* number of chars in the string */
	char		*c;		/* actually longer. chars of the string */
};

/* to be used instead of slsk_msg, slsk_str, char *. */
struct slsk_buf {
	TAILQ_ENTRY(slsk_buf)			 entry;
	char					*buf;
	SLSK_INT				len;
	SLSK_INT				pos;
};

struct slsk_msg {
	TAILQ_ENTRY(slsk_msg)			 entry;
	char					*buf;
	SLSK_INT				 writtenbytes;
	SLSK_INT				 leftbytes;
	SLSK_INT				 msglen;
};

struct cslsk_file_attr {
	SLSK_INT				 type;
	SLSK_INT				 value;
};

struct cslsk_file {
	SLSK_BYTE				 code;
	char					*filename;
	SLSK_INT				 size1;
	SLSK_INT				 size2;
	char					*ext;
	SLSK_INT				 nattr;
	struct cslsk_file_attr			*attr;
	FILE					*stream;
};

struct cslsk_directory {
	char					*name;
	SLSK_INT				 nfiles;
	struct cslsk_file			*files;
};

struct cslsk_filelist {
	SLSK_INT				 ndirs;
	struct cslsk_directory			*dirs;
};

struct cslsk_conf {
        TAILQ_HEAD(slsk_conns, slsk_conn)	 conns;
        TAILQ_HEAD(slsk_peers, slsk_peer)	 peers;
	TAILQ_HEAD(slsk_searches, slsk_search)   searches;
	TAILQ_HEAD(slsk_transfers, slsk_transfer) transfers;
	struct slsk_conn			*server;
	fd_set					 readfds;
	fd_set					 writefds;
	struct slsk_roomlist			*roomlist;
	struct slsk_info			*info;
        uint8_t					 debug;
	int					 waitport;
	char					*username;
	char					*password;
	int					 nmaxconns;
	int					 nconns;

};

struct slsk_conn {
	TAILQ_ENTRY(slsk_conn)			 entry;
	TAILQ_HEAD(slsk_msgs, slsk_msg)		 msgs;
	int					 fd;

	struct slsk_buf				*rbuf;

	char					 type;
	SLSK_INT				 token;

	uint32_t				 status;
	struct slsk_peer			*peer;
	struct slsk_transfer			*transfer;
};


struct slsk_peer {
	TAILQ_ENTRY(slsk_peer)			 entry;
//	TAILQ_HEAD(slsk_msgs, slsk_msg)		 msgs;
//	TAILQ_HEAD(slsk_conns, slsk_peerconn)	 conns;

	char					*hostname;
	int					 port;
	struct sockaddr_in			 addr;

	char					*username;
	char					*desc;
	struct cslsk_filelist			*filelist;
	SLSK_BYTE				 nslots;
	SLSK_INT				 speed;
	SLSK_INT				 qlen;
	SLSK_INT				 nupload;

	int					 (*prs)();
	char					*(*getmsgname)();
};

struct slsk_transfer {
	TAILQ_ENTRY(slsk_transfer)		 entry;
	struct slsk_conn			*conn;
	struct cslsk_file			*file;
	SLSK_INT				 token;
	SLSK_INT				 direction;
	SLSK_INT				 status;
	SLSK_BYTE				 allowed;
	char					*reason;
};

struct slsk_result {
	TAILQ_ENTRY(slsk_result)		 entry;
	struct slsk_peer			*peer;
	SLSK_INT				 nfiles;
	struct cslsk_file			*files;
};

struct slsk_search {
	TAILQ_ENTRY(slsk_search)		 entry;
	SLSK_INT				 token;
	char					*string;
	TAILQ_HEAD(slsk_results, slsk_result)	 results;
};

struct slsk_roomlist {
	SLSK_INT nrooms;
	char** rooms;
	SLSK_INT* usercnts;
};

struct slsk_userlist {

};

struct slsk_info {
	
};

#define VERSION_STRING "0.00"

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

#ifndef STDIN
#define STDIN 0
#endif

void selectloop (struct cslsk_conf *);

#endif /* __CSLSK_CSLSK_H */
