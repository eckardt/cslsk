/*      $Id: netio.c,v 1.15 2005/02/09 17:10:02 se Exp $ */

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


#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>

#include "interface.h"
#include "prsmsg.h"
#include "prpmsg.h"
#include "netio.h"
#include "msg.h"

int conn_init (char* srvhost, uint16_t srvport)
{
	int sock;
	struct sockaddr_in dest_addr;

//	printf ("Trying to connect to %s:%d...\n", srvhost, srvport);

	sock = socket (PF_INET, SOCK_STREAM, 6);	/* 6 is TCP */
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons (srvport);
	dest_addr.sin_addr.s_addr = inet_addr (srvhost);
	memset(&(dest_addr.sin_zero), '\0', 8);

	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		printf("could not set fd non-blocking\n");
	
	connect (sock, (struct sockaddr*) &dest_addr, sizeof (struct sockaddr));

	return (sock);
}

/* takes a tailq of connections, creates a new connection
 * and appends it to the tailq */
struct slsk_conn *addconn(struct slsk_conns *conns)
{
	struct slsk_conn *conn;

	conn = malloc(sizeof(struct slsk_conn));
	conn->rbuf = malloc(sizeof(struct slsk_buf));
	conn->rbuf->buf = NULL;
	conn->rbuf->pos = 0;
	conn->rbuf->len = 0;
	conn->fd = 0;
	conn->type = '\0';
	conn->token = 0;
	conn->status = 0;
	conn->peer = NULL;
	conn->transfer = NULL;

	TAILQ_INIT(&conn->msgs);

	if (conns != NULL) {
		if(TAILQ_EMPTY(conns)) {
			TAILQ_INIT(conns);
			TAILQ_INSERT_HEAD(conns, conn, entry);
		} else
			TAILQ_INSERT_TAIL(conns, conn, entry);
	}

	return conn;
}

/* creates a new peer */
struct slsk_peer *addpeer(char *hostname, int port, char *username,
		SLSK_BYTE nslots, SLSK_INT speed, SLSK_INT qlen, struct slsk_peers *peers)
{
	struct slsk_peer *peer;

	peer = malloc(sizeof(struct slsk_peer));
	peer->prs = prpmsg;
	peer->getmsgname = getpmsgname;

	peer->port = port;
	peer->nslots = nslots;
	peer->speed = speed;
	peer->qlen = qlen;
	peer->filelist = NULL;

	if (hostname != NULL) {
		peer->hostname = malloc(strlen(hostname)+1);
		strncpy(peer->hostname, hostname, strlen(hostname)+1);
	} else
		peer->hostname = NULL;

	if (username != NULL) {
		peer->username = malloc(strlen(username)+1);
		strncpy(peer->username, username, strlen(username)+1);
	} else
		peer->username = NULL;

	if (peers != NULL) {
		if(TAILQ_EMPTY(peers)) {
			TAILQ_INIT(peers);
			TAILQ_INSERT_HEAD(peers, peer, entry);
		} else
			TAILQ_INSERT_TAIL(peers, peer, entry);
	}

	return peer;
}

int slsk_connect(struct slsk_conn *conn)
{
	int rv;
	int err;
	if (!conn->fd) {
//		printf("Trying to connect to %s:%d...\n", conn->peer->hostname,
//				conn->peer->port);
		conn->fd = socket (PF_INET, SOCK_STREAM, 6);	/* 6 is TCP */
		conn->peer->addr.sin_family = AF_INET;
		conn->peer->addr.sin_port = htons (conn->peer->port);
		conn->peer->addr.sin_addr.s_addr = inet_addr (conn->peer->hostname);
		memset(&(conn->peer->addr.sin_zero), '\0', 8);

		if (fcntl(conn->fd, F_SETFL, O_NONBLOCK) < 0)
			printf("could not set fd non-blocking\n");
	}
			
	rv = connect (conn->fd, (struct sockaddr *) &conn->peer->addr,
			sizeof(struct sockaddr));
	err = errno;
	return rv;
}

int conn_dstr (int sock)
{
//	printf ("Trying to close socket...");
	sock = close (sock);

/*	if (!sock) 
		printf (" successful.\n");
	else
		printf (" failed.\n");*/

	return (sock);
}

/*char *
recv_msg(struct slsk_conn *server)
{
	int rb;
		if ((rb = read (server->fd, &server->msglen, INTSIZE)) != INTSIZE) {
			printf ("this socket is dead.\n");
		}
		server->readbytes = 0;
		server->leftbytes = server->msglen;

		server->rbuf = (char *) malloc(server->msglen + CODE_OFFSET + INTSIZE);
		int2msg(server->rbuf, server->msglen);

		server->readbytes = read(server->fd, server->rbuf + CODE_OFFSET,
				server->leftbytes);
		server->leftbytes = server->msglen - server->readbytes;

	} else {
		rb = read(server->fd, server->rbuf + CODE_OFFSET +
				server->readbytes, server->leftbytes);
		if (!rb) {
			printf ("this socket is dead.\n");
		}
		server->readbytes += rb;
		server->leftbytes = server->msglen - server->readbytes;
	}

		server->msglen = 0;
		server->readbytes = 0;
		server->leftbytes = 0;
		
	} else
		return NULL;
}
*/

int getmsg(struct slsk_conn *conn, struct cslsk_conf *conf)
{
	int rcode = 0;
	int rb;

/*	if (peer->msglen) {
		printf ("getmsg: %s:%i %u\t", peer->hostname, peer->port, peer->token);
		printf (" msg: r:%i l:%i m:%i\n", peer->readbytes, peer->leftbytes, peer->msglen);
	}*/
	
	if (conn->type == 'F')
		printf("Trying to read from F-type conn!\n");
	if (conn->rbuf->pos + 1 >= conn->rbuf->len) {		/* no message to complete receiving */
//		free(server->rbuf);		/* so receive a new message */
		rb = read(conn->fd, &conn->rbuf->len, INTSIZE);
		if (rb == 0 || rb == -1) {
			/* uh oh, error. */
			warn ("getmsg: socket is dead:");
			conn->status = conn->status & ~SLSK_PSTAT_CONNECTED;
			FD_CLR(conn->fd, &conf->readfds);
			FD_CLR(conn->fd, &conf->writefds);
			conn_dstr(conn->fd);
			conn->fd = 0;
			conf->nconns--;
			return 0;
		} else {
			conn->rbuf->pos = 0;

			conn->rbuf->buf = (char *) malloc(conn->rbuf->len + CODE_OFFSET);
			int2msg(conn->rbuf, conn->rbuf->len);

//			conn->readbytes = read(conn->fd, conn->rbuf + CODE_OFFSET,
//					conn->leftbytes);
//			conn->leftbytes = conn->msglen - conn->readbytes;
		}

	}

	rb = read(conn->fd, conn->rbuf->buf + conn->rbuf->pos + CODE_OFFSET, (conn->rbuf->len - conn->rbuf->pos - 1));
	if (rb == 0 || rb == -1) {
		/* uh oh, error */
		warn ("getmsg: socket is dead:");
		conn->status = conn->status & ~SLSK_PSTAT_CONNECTED;
		FD_CLR(conn->fd, &conf->readfds);
		FD_CLR(conn->fd, &conf->writefds);
		conn_dstr(conn->fd);
		conn->fd = 0;
		conf->nconns--;
		return 0;
	} else {
		conn->rbuf->pos += rb;
//		printf ("getmsg: got %i bytes!\n", rb);
		if (conn->type == 'F') {
			printf ("got %i bytes from F-type conn!\n", rb);
			if (!conn->transfer->file->stream)
				conn->transfer->file->stream = fopen("DOWNLOAD", "w");
			if (conn->transfer->file->stream) {
		        if (fwrite((char *)(conn->rbuf->buf + conn->rbuf->pos + CODE_OFFSET - rb), 1, rb, conn->transfer->file->stream) < rb)
				printf("Error writing to file!\n");
			if (fflush(conn->transfer->file->stream) == EOF)
				printf("Error flushing file!\n");
			}
		}
	}

	if ((conn->rbuf->pos + 1 >= conn->rbuf->len) && conn->fd) {	/* completed msg in one read() */
		conn->rbuf->len = 0;
		conn->rbuf->pos = 0;
		if ((rcode = (*conn->peer->prs)(conn->rbuf, conn, conf))) {
			warn("could not parse %s message(%i) from server",
					(*conn->peer->getmsgname)(rcode), rcode);
//			dump_msg(conn->rbuf);
		}
		
	} else
		return 0;

	return rcode;
}

int putmsg(struct slsk_conn *conn, struct cslsk_conf *conf)
{
	struct slsk_msg *msg;
	int wb;

	msg = TAILQ_FIRST(&conn->msgs);
	if (msg != NULL) {	/* there are messages in the queue to send */
/*		printf ("putpmsg: %s:%i %u\t", peer->hostname, peer->port, peer->token);
		printf (" msg: w:%i l:%i m:%i\n", msg->writtenbytes, msg->leftbytes, msg->msglen);*/

		if (conn->type == 'F')
			printf("trying to write to a F-conn\n");
		wb = write(conn->fd, msg->buf + msg->writtenbytes, msg->leftbytes);
		if (wb != -1) {
			msg->writtenbytes += wb;
			if (conn->type == 'F')
				printf("wrote something to a F-conn (%i)!\n", wb);
//			printf("putmsg: wrote %i/%i bytes\n", wb, msg->leftbytes);
			msg->leftbytes = msg->msglen - msg->writtenbytes;

			if (!msg->leftbytes) {
				/* the message has been sent completely */
				dump_msg(msg);
				TAILQ_REMOVE(&conn->msgs, msg, entry);
//				printf("putmsg: completed writing message\n");
				/* so delete it from the head of the queue */
/*		free(msg->buf);*/
			} 
		} else {
			warn ("putmsg: failed write():");
			conn->status = conn->status & ~SLSK_PSTAT_CONNECTED;
			FD_CLR(conn->fd, &conf->readfds);
			FD_CLR(conn->fd, &conf->writefds);
			conn_dstr(conn->fd);
			conn->fd = 0;
			conf->nconns--;
		}
	}
	return 0;
}

/* this takes a PREVIOUSLY created msg and builds a "slsk_msg" type msg from it */
struct slsk_msg *postmkmsg (struct slsk_buf *buf)
{
	struct slsk_msg *newmsg;

	if ((newmsg = malloc(sizeof(struct slsk_msg))) != NULL) {
		newmsg->msglen = buf->len;
		newmsg->writtenbytes = 0;
		newmsg->leftbytes = buf->len;
		newmsg->buf = buf->buf;
		return newmsg;
	} else
		return NULL;
}

/* this takes a PREVIOUSLY created "slsk_msg" type msg and appends it to the q */
int basic_msg2q (struct slsk_msg *smsg, struct slsk_msgs *msgq)
{
	if (!msgq) {
		msgq = malloc(sizeof(struct slsk_msgs));
		TAILQ_INIT(msgq);
	}
	if (TAILQ_EMPTY(msgq)) {
		TAILQ_INIT(msgq);
		TAILQ_INSERT_HEAD(msgq, smsg, entry);
	} else
		TAILQ_INSERT_TAIL(msgq, smsg, entry);
	return 0;
}

/* this takes a char *msg, builds a "struct slsk_msg" type msg and appends it to the q */
/* msg2q inserts a msg to a msgq, making it ready for being sent */
int msg2q (struct slsk_buf *buf, struct slsk_msgs *msgq)
{
	struct slsk_msg *newmsg;

	if ((newmsg = postmkmsg(buf)) != NULL) {
		if (!basic_msg2q(newmsg, msgq)) return 0;
		else return 1;
	} else
		return 1;
}

