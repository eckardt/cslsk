/*      $Id: interface.c,v 1.9 2005/02/09 17:10:01 se Exp $ */

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
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "cslsk.h"
#include "msg_codes.h"
#include "mksmsg.h"
#include "interface.h"
#include "netio.h"
#include "msg.h"
#include "commands.h"


void eval_input (char* inputstring, struct cslsk_conf *conf)
{
	char **ap, *argv[10];
	int i;

	for (i=0;i<9;i++)
		argv[i] = NULL;

	for (ap = argv; ap < &argv[9] &&
	    (*ap = strsep(&inputstring, " \t")) != NULL;) {
		if (**ap != '\0')
		ap++;
	}
	if (!argv[0]) argv[0] = "";

//	printf("|%s|%s|%s|%s|\n", argv[0], argv[1], argv[2], argv[3]);

	if (!strcmp (argv[0], CMD_LOGIN))
		do_login(argv, conf, &conf->server->msgs);
	else if (!strcmp (argv[0], CMD_EXIT))
		do_exit(argv);
	else if (!strcmp (argv[0], CMD_LOGOUT))
		do_logout(argv, conf);

	else if (!strcmp(argv[0], CMD_PEERCONN))
		do_peerconn(argv, conf);

	else if (!strcmp(argv[0], CMD_SEARCH))
		do_search(argv, &conf->server->msgs, &conf->searches);
	else if (!strcmp(argv[0], CMD_LISTSEARCHES))
		do_listsearches(argv, &conf->searches);
	else if (!strcmp(argv[0], CMD_LISTTRANSFERS))
		do_listtransfers(argv, &conf->transfers);

	else if (!strcmp(argv[0], CMD_DOWNLOAD))
		do_download(argv, conf, &conf->searches);
	
	else if (!strcmp(argv[0], CMD_GETFILELIST))
		do_getfilelist(argv, conf);
	else if (!strcmp(argv[0], CMD_SHOWFILELIST))
		do_showfilelist(argv, conf);

	else if (!strcmp (argv[0], CMD_UPDATEROOMLIST))
		do_updateroomlist(argv, conf, &conf->server->msgs);
	else if (!strcmp (argv[0], CMD_ROOMLIST))
		do_roomlist(argv, conf->roomlist);
	else if (!strcmp (argv[0], CMD_JOINROOM))
		do_joinroom(argv, conf, &conf->server->msgs);
	else if (!strcmp (argv[0], CMD_LEAVEROOM))
		do_leaveroom(argv, conf, &conf->server->msgs);
	else if (!strcmp(argv[0], CMD_SAYCHATROOM))
		do_saychatroom(argv, &conf->server->msgs);

	else if (!strcmp (argv[0], CMD_STATUS))
		do_status(argv, conf);
	else
		printf ("unknown command\n");
}

void do_joinroom (char *argv[], struct cslsk_conf *conf, struct slsk_msgs *msgs)
{
	if (argv[1])
		msg2q(mksmsg("lis", MSG_JOINROOM, argv[1]), msgs);
}

void do_leaveroom (char* argv[], struct cslsk_conf *conf, struct slsk_msgs *msgs)
{
	if (argv[1])
		msg2q(mksmsg("lis", MSG_LEAVEROOM, argv[1]), msgs);
}

void do_saychatroom (char *argv[], struct slsk_msgs *msgs)
{
	int i, l=0;
	char *str = malloc(100);

	if (argv[1]) {
		for (i=2; i<10 && argv[i] != NULL; i++) {
			strcpy(str+l,argv[i]);
			l+=strlen(argv[i]);
			strcpy(str+l," ");
			l+=1;
		}
		str[l-1] = 0;
		msg2q(mksmsg("liss", MSG_SAYCHATROOM, argv[1], str), msgs);
	}
		
}

void do_login (char *argv[], struct cslsk_conf *conf, struct slsk_msgs *msgs)
{
	if (argv[1] && argv[2]) {
		conf->username = argv[1];
		conf->password = argv[2];
	}
	
	slsk_connect(conf->server);

	msg2q(mksmsg("lissisi", MSG_LOGIN, conf->username, conf->password,
				CLIENT_VERSION, "cd8fe449c848b51544f0d112319816cc", 1), msgs);
	msg2q(mksmsg("lii", MSG_SETWAITPORT, conf->waitport), msgs);
	msg2q(mksmsg("lis", MSG_GETUSERSTATS, conf->username), msgs);
	msg2q(mksmsg("liii", MSG_SHAREDFOLDERSFILES, 0,0), msgs);
	msg2q(mksmsg("lib", MSG_GETPARENTLIST, 1), msgs);

	FD_SET(conf->server->fd, &conf->writefds);

	printf (">>>> Logging in...\n");
}

void do_logout (char *argv[], struct cslsk_conf *conf)
{
	FD_CLR(conf->server->fd, &conf->readfds);
	FD_CLR(conf->server->fd, &conf->writefds);
	conf->server->fd = conn_dstr (conf->server->fd);
	conf->server->fd = 0;
	printf (">>>> Logged out...\n");
}

void do_exit (char *argv[])
{
	printf (">>>> Bye.\n");
	exit (0);
}

void do_roomlist (char *argv[], struct slsk_roomlist *roomlist)
{
	int i;

	printf (">>>> There are currently %i rooms.\n", roomlist->nrooms);
	for (i = 0; i < roomlist->nrooms; i++) {
		fprintf (stderr, "%s [%i Users]\n", roomlist->rooms[i], roomlist->usercnts[i]);
	}
}

void do_updateroomlist (char *argv[], struct cslsk_conf *conf, struct slsk_msgs *msgs)
{
	printf (">>>> Asking for new roomlist...\n");
	msg2q (mksmsg("li", MSG_ROOMLIST), msgs);
}

void do_status (char *argv[], struct cslsk_conf *conf)
{
	struct slsk_conn *conn;

	printf ("Logged in: %s\n",
			(conf->server->status & SLSK_PSTAT_CONNECTED) ? "yes" : "no");
	
	TAILQ_FOREACH(conn, &conf->conns, entry) {
		printf ("--conn %s:%i\ttkn:%i\tC:%i\tT:%c\tu:%s\tq:%i\n",
				conn->peer->hostname, conn->peer->port, conn->token,
				(conn->status & SLSK_PSTAT_CONNECTED) ? 1:0,
				conn->type, conn->peer->username, conn->peer->qlen);
	}
}

void do_listsearches (char *argv[], struct slsk_searches *searches)
{
	struct slsk_search *search;
	struct slsk_result *result;
	SLSK_INT i;
	int noqueued;
	
	if (argv[3] && !strcmp(argv[3], "-q"))
		noqueued = 1;
	else
		noqueued = 0;
	printf("-----\n");
	TAILQ_FOREACH(search, searches, entry) {
		printf("searchstring: %s, token: %i\n", search->string, search->token);
		if (argv[1] &&
			(!strcmp(argv[1], "*") || (search->token == atoi(argv[1])))) {
			TAILQ_FOREACH(result, &search->results, entry) {
				if (argv[2] &&
					(!strcmp(argv[2], "*") ||
					 (!strcmp(result->peer->username, argv[2])))) {
					printf("+username: %s, nfiles: %i, qlen: %i\n",
						result->peer->username, result->nfiles,
						result->peer->qlen);
					for (i = 0; i < result->nfiles; i++)
					 if (!noqueued || (result->peer->qlen < 10))
					 printf("%i\t%s (%ib)\n", i, 
					 result->files[i].filename, result->files[i].size1);
				} else {
					printf("-username: %s, nfiles: %i, qlen: %i\n",
						result->peer->username, result->nfiles,
						result->peer->qlen);
				}
			}
		}
	}
	printf("-----\n");
}

void do_listtransfers (char *argv[], struct slsk_transfers *transfers)
{
	struct slsk_transfer *transfer;

	printf("-----\n");
	TAILQ_FOREACH(transfer, transfers, entry) {
		printf ("u:%s f:%s t:%i D:%i A:%i S:%i r:%s\n", transfer->conn->peer->username,
				transfer->file->filename, transfer->token, transfer->direction,
				transfer->allowed, transfer->status, transfer->reason);
	}
	printf("-----\n");
}

struct cslsk_file *newfile(struct cslsk_file *file, struct cslsk_directory *dir)
{
	struct cslsk_file *nf = malloc(sizeof(struct cslsk_file));
	SLSK_INT fslen, dslen;

	memcpy(nf, file, sizeof(struct cslsk_file));
	fslen = strlen(file->filename);
	dslen = strlen(dir->name);
	nf->filename = malloc(fslen + dslen + 10);
	strncpy(nf->filename, dir->name, dslen);
	strncat(nf->filename, "\\", 1);
	strncat(nf->filename, file->filename, fslen);
	return nf;
}

void do_download(char *argv[], struct cslsk_conf *conf, struct slsk_searches *searches)
{
	struct slsk_peer *fpeer, *peer = NULL;
	struct slsk_conn *fconn = NULL, *conn;
	struct slsk_search *search;
	struct slsk_result *result;
	struct slsk_transfer *transfer;
	struct cslsk_file *file;

	  if (argv[1] && (!strcmp(argv[1], "p"))) {
	   if (argv[2])
	     TAILQ_FOREACH(fpeer, &conf->peers, entry)
		     if (!strncmp(argv[2], fpeer->username, strlen(fpeer->username)))
			     peer = fpeer;
	   if (peer && peer->filelist && argv[3] && argv[4]) {
		   if (peer->filelist->ndirs > atoi(argv[3]) && peer->filelist->dirs[atoi(argv[3])].nfiles > atoi(argv[4])) {
			   file = newfile(&peer->filelist->dirs[atoi(argv[3])].files[atoi(argv[4])], &peer->filelist->dirs[atoi(argv[3])]);
			   printf ("Filename: %s\n", file->filename);
		   } else
			   printf ("That file doesn't exist\n");
	   } else
		   printf ("Don't know that user / User has no file list / too few args\n");
	  } else
	TAILQ_FOREACH(search, searches, entry) {
	  if (argv[1] && (!strcmp(argv[1], "*") || (search->token == atoi(argv[1])))) {
	    TAILQ_FOREACH(result, &search->results, entry) {
	      if (argv[2] && argv[3] && (!strcmp(argv[2], "*") ||
			(!strncmp(result->peer->username, argv[2], strlen(argv[2]))))) {
		      peer = result->peer;
		      file = &result->files[atoi(argv[3])];
/* transferrequest      "liiisi" -> int dir-constant, int token, str filename, int filesize
	 */
	      }
	    }
	  }
	}

	if (peer && file) {
		TAILQ_FOREACH(fconn, &conf->conns, entry) {
		/* now find unused P-type connection to this peer */
			if (fconn->peer == peer && fconn->type == 'P')
			/* yap, we found one: use it! */
				conn = fconn;
		}
		if (!conn) {
		/* we haven't found a p-type conn, so we need to open a new one */
			conn = addconn(&conf->conns);
			conn->peer = peer;
			conn->type = 'P';
			conn->token = 24;
			slsk_connect(conn);
			FD_SET(conn->fd, &conf->writefds);
			msg2q(mksmsg("lbssi", PMSG_PEERINIT, conf->username, "P", conn->token), &conn->msgs);
		}
		/* next send a transferrequest */

		transfer = malloc(sizeof(struct slsk_transfer));
		transfer->conn = conn;
		transfer->file = file;
		transfer->token = 24;
		transfer->direction = SLSK_CONST_DOWNLOAD;

		if (TAILQ_EMPTY(&conf->transfers)) {
			TAILQ_INIT(&conf->transfers);
			TAILQ_INSERT_HEAD(&conf->transfers, transfer, entry);
		} else
			TAILQ_INSERT_TAIL(&conf->transfers, transfer, entry);

		msg2q(mksmsg("lis", PMSG_PLACEHOLDUPLOAD, file->filename), &conn->msgs);
		msg2q(mksmsg("liiis", PMSG_TRANSFERREQUEST, transfer->direction,
					transfer->token, file->filename), &conn->msgs);
	}
}

void do_peerconn(char* argv[], struct cslsk_conf *conf)
{
	struct slsk_conn *conn;

	if (argv[1] && argv[2] && argv[3] && argv[4] && argv[5]) {
		conn = addconn(&conf->conns);
		conn->peer = addpeer(argv[1], atoi(argv[2]), argv[5], 0, 0, 0, &conf->peers);

		conn->type = argv[3][0];
		conn->token = atoi(argv[4]);

		slsk_connect(conn);

		FD_SET(conn->fd, &conf->writefds);

		msg2q(mksmsg("lbssi", PMSG_PEERINIT, conf->username, argv[3], conn->token), &conn->msgs);
	}
}

void do_getfilelist(char *argv[], struct cslsk_conf *conf)
{
	struct slsk_conn *fconn, *conn = NULL;
	struct slsk_peer *fpeer, *peer = NULL;

	if (argv[1]) {
		TAILQ_FOREACH(fpeer, &conf->peers, entry) {
			if (!strncmp(fpeer->username, argv[1], strlen(argv[1])))
				peer = fpeer;
		}

		if (peer) {
			TAILQ_FOREACH(fconn, &conf->conns, entry) {
				if ((fconn->peer == peer) &&
						fconn->type == 'P')
				/* we found a p-type connection to this user */
					conn = fconn;
			}
			if (!conn) {
			/* hmmm... we lost a connection to that user.. we must reconnect */
			conn = addconn(&conf->conns);
			conn->peer = peer;
	
			conn->type = 'P';
			conn->token = 23;
	
			slsk_connect(conn);
	
			FD_SET(conn->fd, &conf->writefds);
		
			msg2q(mksmsg("lbssi", PMSG_PEERINIT, conf->username, "P", conn->token), &conn->msgs);
			}

			msg2q(mksmsg("li", PMSG_GETSHAREDFILELIST), &conn->msgs);
		}
		else
			printf("Huh? I don't know that user...\n");
	}
}

void do_showfilelist(char *argv[], struct cslsk_conf *conf)
{
	struct slsk_peer *fpeer, *peer = NULL;
	SLSK_INT idirs, ifiles;

	if (argv[1]) {
		TAILQ_FOREACH(fpeer, &conf->peers, entry) {
			if (!strncmp(fpeer->username, argv[1], strlen(argv[1])))
				peer = fpeer;
		}
		if (peer)
		  if (peer->filelist) {
		    printf("-----\n");
		    for (idirs = 0; idirs < peer->filelist->ndirs; idirs++) {
	             for (ifiles = 0; ifiles < peer->filelist->dirs[idirs].nfiles; ifiles++)
			  printf("%i-%i\t%s (%ib)\n", idirs, ifiles,
				   peer->filelist->dirs[idirs].files[ifiles].filename,
				   peer->filelist->dirs[idirs].files[ifiles].size1);
		    }
		    printf("-----\n");
		  }
		  else
			  printf("I don't have a filelist of that user...\n");
		else
			printf("Huh? I don't know that user...\n");
	}
}

void do_search(char *argv[], struct slsk_msgs *msgs, struct slsk_searches *searches)
{
	int i, l=0;
	char *str = malloc(100);
	struct slsk_search *search;

	if (argv[1] != NULL) {
		for (i=2; i<10 && argv[i] != NULL; i++) {
			strcpy(str+l,argv[i]);
			l+=strlen(argv[i]);
			strcpy(str+l," ");
			l+=1;
		}
		str[l-1] = 0;
		msg2q(mksmsg("liis", MSG_FILESEARCH, atoi(argv[1]), str), msgs);
		search = malloc(sizeof(struct slsk_search));
		search->token = atoi(argv[1]);
		search->string = str;
		TAILQ_INIT(&search->results);

		if (TAILQ_EMPTY(searches)) {
			TAILQ_INIT(searches);
			TAILQ_INSERT_HEAD(searches, search, entry);
		} else
			TAILQ_INSERT_TAIL(searches, search, entry);
		
	}
}

char* getmsgname (uint32_t code)
{
	switch (code) {
	case MSG_LOGIN:
		return "LOGIN"; break;
	case MSG_SETWAITPORT:
		return "SETWAITPORT"; break;
	case MSG_GETPEERADDRESS:
		return "GETPEERADDRESS"; break;
	case MSG_ADDUSER:
		return "ADDUSER"; break;
	case MSG_UNKNOWN1:
		return "UNKNOWN1"; break;
	case MSG_GETUSERSTATUS:
		return "GETUSERSTATUS"; break;
	case MSG_SAYCHATROOM:
		return "SAYCHATROOM"; break;
	case MSG_JOINROOM:
		return "JOINROOM"; break;
	case MSG_LEAVEROOM:
		return "LEAVEROOM"; break;
	case MSG_USERJOINEDROOM:
		return "USERJOINEDROOM"; break;
	case MSG_USERLEFTROOM:
		return "USERLEFTROOM"; break;
	case MSG_CONNECTTOPEER:
		return "CONNECTTOPEER"; break;
	case MSG_MESSAGEUSER:
		return "MESSAGEUSER"; break;
	case MSG_MESSAGEACKED:
		return "MESSAGEACKED"; break;
	case MSG_FILESEARCH:
		return "FILESEARCH"; break;
	case MSG_SETSTATUS:
		return "SETSTATUS"; break;
	case MSG_SERVERPING:
		return "SERVERPING"; break;
	case MSG_SENDSPEED:
		return "SENDSPEED"; break;
	case MSG_SHAREDFOLDERSFILES:
		return "SHAREDFOLDERSFILES"; break;
	case MSG_GETUSERSTATS:
		return "GETUSERSTATS"; break;
	case MSG_QUEUEDDOWNLOADS:
		return "QUEUEDDOWNLOADS"; break;
	case MSG_RELOGGED:
		return "RELOGGED"; break;
	case MSG_PLACEINLINERESPONSE:
		return "PLACEINLINERESPONSE"; break;
	case MSG_ROOMADDED:
		return "ROOMADDED"; break;
	case MSG_ROOMREMOVED:
		return "ROOMREMOVED"; break;
	case MSG_ROOMLIST:
		return "ROOMLIST"; break;
	case MSG_EXACTFILESEARCH:
		return "EXACTFILESEARCH"; break;
	case MSG_ADMINMESSAGE:
		return "ADMINMESSAGE"; break;
	case MSG_GLOBALUSERLIST:
		return "GLOBALUSERLIST"; break;
	case MSG_TUNNELEDMESSAGE:
		return "TUNNELEDMESSAGE"; break;
	case MSG_PRIVILEGEDUSERS:
		return "PRIVILEGEDUSERS"; break;
	case MSG_GETPARENTLIST:
		return "GETPARENTLIST"; break;
	case MSG_UNKNOWN2:
		return "UNKNOWN2"; break;
	case MSG_PARENTINACTIVITYTIMEOUT:
		return "PARENTINACTIVITYTIMEOUT"; break;
	case MSG_SEARCHINACTIVITYTIMEOUT:
		return "SEARCHINACTIVITYTIMEOUT"; break;
	case MSG_MINPARENTSINCACHE:
		return "MINPARENTSINCACHE"; break;
	case MSG_DISTRIBALIVEINTERVAL:
		return "DISTRIBALIVEINTERVAL"; break;
	case MSG_ADDTOPRIVILEGED:
		return "ADDTOPRIVILEGED"; break;
	case MSG_CHECKPRIVILEGES:
		return "CHECKPRIVILEGES"; break;
	case MSG_CANTCONNECTTOPEER:
		return "CANTCONNECTTOPEER"; break;
	case MSG_SEARCHREQUEST:
		return "SEARCHREQUEST"; break;
	case MSG_NETINFO:
		return "NETINFO"; break;
	case MSG_WISHLISTSEARCH:
		return "WISHLISTSEARCH"; break;
	case MSG_WISHLISTINTERVAL:
		return "WISHLISTINTERVAL"; break;
	case MSG_ROOMTICKERSTATE:
		return "ROOMTICKERSTATE"; break;
	case MSG_ROOMTICKERADD:
		return "ROOMTICKERADD"; break;
	case MSG_ROOMTICKERREMOVE:
		return "ROOMTICKERREMOVE"; break;
	case MSG_ROOMTICKERSET:
		return "ROOMTICKERSET"; break;
	case MSG_REMOVEUSER:
		return "REMOVEUSER"; break;
	case MSG_CANTCREATEROOM:
		return "CANTCREATEROOM"; break;
	default:
		return "unknown"; break;
	}
}

char* getpmsgname (SLSK_INT code)
{
	if ((SLSK_BYTE)code == PMSG_PEERINIT)
		return "PEERINIT";
	else if ((SLSK_BYTE)code == PMSG_PIERCEFW)
		return "PIERCEFW";
	else {
	switch (code) {
        case PMSG_SHAREDFILELIST:
                return "SHAREDFILELIST"; break;
        case PMSG_FILESEARCHREQUEST:
                return "FILESEARCHREQUEST"; break;
        case PMSG_FILESEARCHRESULT:
                return "FILESEARCHRESULT"; break;
        case PMSG_USERINFOREQUEST:
                return "USERINFOREQUEST"; break;
        case PMSG_USERINFOREPLY:
                return "USERINFOREPLY"; break;
        case PMSG_FOLDERCONTENTSREQUEST:
                return "FOLDERCONTENTSREQUEST"; break;
        case PMSG_FOLDERCONTENTSRESPONSE:
                return "FOLDERCONTENTSRESPONSE"; break;
        case PMSG_TRANSFERREQUEST:
                return "TRANSFERREQUEST"; break;
        case PMSG_TRANSFERRESPONSE:
                return "TRANSFERRESPONSE"; break;
        case PMSG_PLACEHOLDUPLOAD:
                return "PLACEHOLDUPLOAD"; break;
        case PMSG_QUEUEUPLOAD:
                return "QUEUEUPLOAD"; break;
        case PMSG_PLACEINQUEUE:
                return "PLACEINQUEUE"; break;
        case PMSG_UPLOADFAILED:
                return "UPLOADFAILED"; break;
        case PMSG_QUEUEFAILED:
                return "QUEUEFAILED"; break;
	default:
		return "unknown"; break;
	}
	}
}

