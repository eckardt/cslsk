/*      $Id: cslsk.c,v 1.11 2005/02/09 17:10:01 se Exp $ */

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
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <err.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "cslsk.h"
#include "interface.h"
#include "msg.h"
#include "mksmsg.h"
#include "prsmsg.h"
#include "prpmsg.h"
#include "netio.h"

int main (int argc, char* argv[])
{
	struct cslsk_conf	conf;

	TAILQ_INIT(&conf.conns);
	TAILQ_INIT(&conf.peers);
	TAILQ_INIT(&conf.searches);
	TAILQ_INIT(&conf.transfers);

	conf.waitport = 6666;
	conf.debug = 1;
	conf.roomlist = malloc(sizeof(struct slsk_roomlist));

	conf.server = addconn(NULL);
	conf.server->peer = addpeer("38.115.131.131", 2240, NULL, 0, 0, 0, NULL);
	conf.server->peer->getmsgname = getmsgname;
	conf.server->peer->prs = prsmsg;

	conf.username = "cslskuser";
	conf.password = "";
	conf.nmaxconns = 4;
	conf.nconns = 0;

	signal(SIGPIPE, SIG_IGN);

	selectloop (&conf);

	exit (0);
}

void selectloop (struct cslsk_conf *conf)
{
	int nr = 1; /* number of the ready fds */
	int ret;
	fd_set *rfds, *wfds;
	char *cmd_input = NULL;
	struct slsk_conn *conn;
	struct timeval *timeout;
//	char *argv[10] = {"login", 0};
	/*struct slsk_peer *peer;*/

	rfds = malloc(sizeof(fd_set));
	wfds = malloc(sizeof(fd_set));
	FD_ZERO(&conf->readfds);
	FD_ZERO(&conf->writefds);
	FD_SET(STDIN, &conf->readfds);

	timeout=malloc(sizeof(struct timeval));

//	do_login(argv, conf, &conf->server->msgs);
	while (1) {
		if (!FD_ISSET(STDIN, &conf->readfds))
			FD_SET(STDIN, &conf->readfds);
		*rfds = conf->readfds;
		*wfds = conf->writefds;
		timeout->tv_usec=500;
		timeout->tv_sec=0;
		
		nr = select(16, rfds, wfds, NULL, timeout);

		/* STDIN */
		if (FD_ISSET(STDIN, rfds)) {
			if (cmd_input)
				free(cmd_input);
			cmd_input = readline("> ");
			if (cmd_input && *cmd_input)
				add_history(cmd_input);
			eval_input(cmd_input, conf);
		}

		/* Server stuff */
		if ((conf->server->status & SLSK_PSTAT_CONNECTED)
				&& FD_ISSET(conf->server->fd, rfds)) {
			getmsg(conf->server, conf);
		}
		if (FD_ISSET(conf->server->fd, wfds)) {
			if (!(conf->server->status & SLSK_PSTAT_CONNECTED)) {
				if (((ret = slsk_connect(conf->server)) == 0 ) || (ret == -1 && errno == EISCONN)) {
					FD_SET(conf->server->fd, &conf->readfds);
					conf->server->status = (conf->server->status | SLSK_PSTAT_CONNECTED);
					printf("connected to server!\n");
					putmsg(conf->server, conf);
					/* connect() completed */
				} else {
					printf ("failed connecting to server!\n");
					warn ("slsk_conn:");
					conf->server->status = conf->server->status & ~SLSK_PSTAT_CONNECTED;
					conf->server->fd = 0;
				}
			} else
				putmsg(conf->server, conf);
		}

		/* Peer stuff */
		TAILQ_FOREACH(conn, &conf->conns, entry) {
			if (!conn->fd)
				TAILQ_REMOVE(&conf->conns, conn, entry);
		}
		
		TAILQ_FOREACH(conn, &conf->conns, entry) {
		if ((conn->status & SLSK_PSTAT_CONNECTED) && FD_ISSET (conn->fd, rfds)) {
			getmsg(conn, conf);
		}

		if (FD_ISSET(conn->fd, wfds)) {
			if(!(conn->status & SLSK_PSTAT_CONNECTED)) {
				if (((ret = slsk_connect(conn)) == 0) || (ret == -1 && errno == EISCONN)) {
					FD_SET(conn->fd, &conf->readfds);
					conn->status = (conn->status | SLSK_PSTAT_CONNECTED);
//					printf("new connection to peer! Type: %c\n", conn->type);
					conf->nconns += 1;
					putmsg(conn, conf);
					/* connect() completed */
				} else {
//					warn ("slsk_conn: failed connecting to peer");
					conn->status = conn->status & ~SLSK_PSTAT_CONNECTED;
					FD_CLR(conn->fd, &conf->readfds);
					FD_CLR(conn->fd, &conf->writefds);
					conn_dstr(conn->fd);
					conn->fd = 0;
				}
			} else {
				putmsg(conn, conf);
			}
		}
		}
	}
}
