/*      $Id: prsmsg.c,v 1.15 2005/02/09 17:10:02 se Exp $ */

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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "cslsk.h"
#include "netio.h"
#include "msg.h"
#include "msg_codes.h"
#include "prsmsg.h"
#include "mksmsg.h"

int prsmsg (struct slsk_buf *buf, struct slsk_peer *peer, struct cslsk_conf *conf)
{
	uint32_t code;

	buf->pos = CODE_OFFSET;
	code = msg2int(buf, NULL);
	buf->pos = PLD_OFFSET;

	switch (code) {
	case MSG_LOGIN:
		return prsmsg_login (buf, conf); break;
//	case MSG_SETWAITPORT:
//		return prsmsg_setwaitport (buf); break;
//	case MSG_GETPEERADDRESS:
//		return prsmsg_getpeeraddress (buf, conf); break;
//	case MSG_ADDUSER:
//		return prsmsg_adduser (buf); break;
//	case MSG_GETUSERSTATUS:
//		return prsmsg_getuserstatus (buf); break;
	case MSG_SAYCHATROOM:
		return prsmsg_saychatroom (buf); break;
//	case MSG_JOINROOM:
//		return prsmsg_joinroom (buf); break;
//	case MSG_LEAVEROOM:
//		return prsmsg_leaveroom (buf); break;
	case MSG_USERJOINEDROOM:
		return prsmsg_userjoinedroom (buf, conf); break;
	case MSG_USERLEFTROOM:
		return prsmsg_userleftroom (buf, conf); break;
	case MSG_CONNECTTOPEER:
		return prsmsg_connecttopeer (buf, conf); break;
//	case MSG_MESSAGEUSER:
//		return prsmsg_messageuser (buf); break;
//	case MSG_MESSAGEACKED:
//		return prsmsg_messageacked (buf); break;
//	case MSG_FILESEARCH:
//		return prsmsg_filesearch (buf); break;
//	case MSG_SETSTATUS:
//		return prsmsg_setstatus (buf); break;
//	case MSG_SERVERPING:
//		return prsmsg_serverping (buf); break;
//	case MSG_SENDSPEED:
//		return prsmsg_sendspeed (buf); break;
//	case MSG_SHAREDFOLDERSFILES:
//		return prsmsg_sharedfoldersfiles (buf); break;
//	case MSG_GETUSERSTATS:
//		return prsmsg_getuserstats (buf); break;
//	case MSG_QUEUEDDOWNLOADS:
//		return prsmsg_queueddownloads (buf); break;
	case MSG_RELOGGED:
		return prsmsg_relogged (buf, conf); break;
//	case MSG_PLACEINLINERESPONSE:
//		return prsmsg_placeinlineresponse (buf); break;
//	case MSG_ROOMADDED:
//		return prsmsg_roomadded (buf); break;
//	case MSG_ROOMREMOVED:
//		return prsmsg_roomremoved (buf); break;
	case MSG_ROOMLIST:
		return prsmsg_roomlist (buf, conf->roomlist); break;
//	case MSG_EXACTFILESEARCH:
//		return prsmsg_exactfilesearch (buf); break;
//	case MSG_ADMINMESSAGE:
//		return prsmsg_adminmessage (buf); break;
//	case MSG_GLOBALUSERLIST:
//		return prsmsg_globaluserlist (buf); break;
//	case MSG_TUNNELEDMESSAGE:
//		return prsmsg_tunneledmessage (buf); break;
//	case MSG_PRIVILEGEDUSERS:
//		return prsmsg_privilegedusers (buf); break;
//	case MSG_GETPARENTLIST:
//		return prsmsg_getparentlist (buf); break;
//	case MSG_PARENTINACTIVITYTIMEOUT:
//		return prsmsg_parentinactivitytimeout (buf); break;
//	case MSG_SEARCHINACTIVITYTIMEOUT:
//		return prsmsg_searchinactivitytimeout (buf); break;
//	case MSG_MINPARENTSINCACHE:
//		return prsmsg_minparentsincache (buf); break;
//	case MSG_DISTRIBALIVEINTERVAL:
//		return prsmsg_distribaliveinterval (buf); break;
//	case MSG_ADDTOPRIVILEGED:
//		return prsmsg_addtoprivileged (buf); break;
//	case MSG_CHECKPRIVILEGES:
//		return prsmsg_checkprivileges (buf); break;
//	case MSG_CANTCONNECTTOPEER:
//		return prsmsg_cantconnecttopeer (buf); break;
	case MSG_NETINFO:
		return prsmsg_netinfo (buf, conf); break;
	default:
		return (code);
	}
}

int prsmsg_login (struct slsk_buf *msg, struct cslsk_conf *conf)
{
	char *str;
	uint8_t succflag = 0;
	uint32_t ip;
	succflag = msg2byte (msg, msg);
	if (!succflag) {
		conf->server->status = conf->server->status & ~SLSK_PSTAT_CONNECTED ;
		fprintf (stderr, ">>>>login> Login failed!\n");
	}
	else {
		conf->server->status = conf->server->status | SLSK_PSTAT_CONNECTED;
		str = msg2str (msg, msg);
		ip = msg2int (msg, msg);
		fprintf (stdout, "%s", str);
		fprintf (stdout, ">>>>login> Your IP address is %x\n", ip);
		str = msg2str (msg, msg);
		fprintf (stdout, ">>>>login> Unknown string: %s\n", str);
	}
	return (0);	
}

int prsmsg_roomlist (struct slsk_buf *msg, struct slsk_roomlist *roomlist)
{
	SLSK_INT i;

/*	for (i = 0; i < roomlist->nrooms; i++) {
		free (roomlist->rooms[i]);
		roomlist->rooms[i] = NULL;
	}*/

	roomlist->nrooms = msg2int (msg, msg);

	roomlist->rooms = calloc (roomlist->nrooms, sizeof(char*));
	roomlist->usercnts = calloc (roomlist->nrooms, sizeof(SLSK_INT));
	for (i = 0; i < roomlist->nrooms; i++)
		roomlist->rooms[i] = msg2str(msg, msg);
	for (i = 0; i < roomlist->nrooms; i++)
		roomlist->usercnts[i] = msg2int(msg, msg);

	return (0);
}

int prsmsg_saychatroom(struct slsk_buf *msg)
{
	char *room, *username, *message;

	room = msg2str(msg, msg);
	username = msg2str(msg, msg);
	message = msg2str(msg, msg);

	printf ("<%s#%s> %s \n", room, username, message);

	return 0;
}

int prsmsg_netinfo(struct slsk_buf *msg, struct cslsk_conf *conf)
{
/*	SLSK_INT n, i, ip_addr, port;
	struct in_addr in;
	struct slsk_conn *conn;
	char* str;

	n = msg2int(msg, &msg);
	for (i = 0; i < n; i++) {
		str = msg2str(msg, &msg);
		ip_addr = msg2int(msg, &msg);
		port = msg2int(msg, &msg);
//		printf("netinfo: %s %u %i\n", str, ip_addr, port);
		memset(&in, '\0', sizeof(struct in_addr));
		in.s_addr = (in_addr_t) htonl(ip_addr);
		conn = addconn(&conf->conns);
		conn->peer = addpeer (inet_ntoa((struct in_addr) in), port, NULL, 0, 0, 0);
		conn->type = 'P';
		conn->token = 23;
		if (conf->nconns < conf->nmaxconns) {
			slsk_connect(conn);
			FD_SET(conn->fd, &conf->writefds);
			msg2q(mksmsg("lbssi", PMSG_PEERINIT, conf->username, "P", conn->token), &conn->msgs);
		}
	}
*/
	
	return 0;
}

int prsmsg_relogged(struct slsk_buf *msg, struct cslsk_conf *conf)
{
	printf(">>>> Relogged. You are disconnected.\n");
	FD_CLR(conf->server->fd, &conf->readfds);
	conn_dstr(conf->server->fd);
	conf->server->fd = 0;
	conf->server->status = conf->server->status & ~SLSK_PSTAT_CONNECTED ;
	return 0;
}

int prsmsg_userjoinedroom(struct slsk_buf *msg, struct cslsk_conf *conf)
{
	char* roomname = msg2str(msg, msg);
	char* username = msg2str(msg, msg);
	printf(" %s: %s joined room.\n", roomname, username);
	free(roomname);
	free(username);
	return 0;
}

int prsmsg_userleftroom(struct slsk_buf *msg, struct cslsk_conf *conf)
{
	char* roomname = msg2str(msg, msg);
	char* username = msg2str(msg, msg);
	printf(" %s: %s left room.\n", roomname, username);
	free(roomname);
	free(username);
	return 0;
}

int prsmsg_connecttopeer(struct slsk_buf *msg, struct cslsk_conf *conf)
{
	struct slsk_conn *conn;
	char *username = msg2str(msg, msg);
	char *type = msg2str(msg, msg);
	SLSK_INT ip_addr = msg2int(msg, msg);
	SLSK_INT port = msg2int(msg, msg);
	SLSK_INT token = msg2int(msg, msg);
	struct in_addr in;

	if (conf->nconns >= conf->nmaxconns)
		return 0;

	in.s_addr = (in_addr_t) htonl(ip_addr);

	conn = addconn(&conf->conns);
	conn->peer = addpeer(inet_ntoa((struct in_addr) in), port, username, 0, 0, 0, &conf->peers);
	conn->type = type[0];
	conn->token = token;
	slsk_connect(conn);
	FD_SET(conn->fd, &conf->writefds);

	msg2q(mksmsg("lbi", PMSG_PIERCEFW, token), &conn->msgs);
	msg2q(mksmsg("li", PMSG_USERINFOREQUEST), &conn->msgs);

	return 0;
}
/*
int prsmsg_getpeeraddress(struct slsk_buf *msg, struct cslsk_conf *conf)
{
	struct slsk_peer *peer;
	char *username = msg2str(msg, &msg);
	char *type = msg2str(msg, &msg);
	SLSK_INT ip_addr = msg2int(msg, &msg);
	SLSK_INT port = msg2int(msg, &msg);
	SLSK_INT token = msg2int(msg, &msg);
	struct in_addr in;
	username = NULL;

	in.s_addr = (in_addr_t) htonl(ip_addr);

	peer = addpeer(&conf->peers);
	peer->hostname = inet_ntoa((struct in_addr) in);
	peer->port = port;
	peer->type = type;
	peer->token = token;
	slsk_conn(peer);
	FD_SET(peer->fd, &conf->writefds);

	msg2q(mksmsg("lbi", PMSG_PIERCEFW, token), &peer->msgs);

	return 0;
}
*/
