/*      $Id: prpmsg.c,v 1.8 2005/02/09 17:10:02 se Exp $ */

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
#include <strings.h>
#include <zlib.h>

#include "cslsk.h"
#include "netio.h"
#include "msg.h"
#include "msg_codes.h"
#include "mksmsg.h"
#include "prsmsg.h"
#include "prpmsg.h"

int prpmsg (struct slsk_buf *msg, struct slsk_conn *conn, struct cslsk_conf *conf)
{
	uint32_t code;

	msg->pos = CODE_OFFSET;
	if (msg2byte(msg, NULL) == PMSG_PEERINIT);
//		return prpmsg_peerinit(msg + CODE_OFFSET + BYTESIZE);
	else if (msg2byte(msg, NULL) == PMSG_PIERCEFW);
//		return prpmsg_piercefw(msg + CODE_OFFSET + BYTESIZE);
	else {

	code = msg2int(msg, NULL);
	msg->pos = PLD_OFFSET;

	switch (code) {
          case PMSG_SHAREDFILELIST:
                  return prpmsg_sharedfilelist(msg, conn->peer); break;
//        case PMSG_FILESEARCHREQUEST:
//                return prpmsg_filesearchrequest(msg, conf); break;
          case PMSG_FILESEARCHRESULT:
                  return prpmsg_filesearchresult(msg, conn->peer, conf); break;
//        case PMSG_USERINFOREQUEST:
//                return prpmsg_userinforequest(msg, conf); break;
          case PMSG_USERINFOREPLY:
                  return prpmsg_userinforeply(msg, conn->peer); break;
//        case PMSG_FOLDERCONTENTSREQUEST:
//                return prpmsg_foldercontentsrequest(msg, conf); break;
//        case PMSG_FOLDERCONTENTSRESPONSE:
//                return prpmsg_foldercontentsresponse(msg, conf); break;
//        case PMSG_TRANSFERREQUEST:
//                return prpmsg_transferrequest(msg, conf); break;
          case PMSG_TRANSFERRESPONSE:
                  return prpmsg_transferresponse(msg, conn, conf); break;
//        case PMSG_PLACEHOLDUPLOAD:
//                return prpmsg_placeholdupload(msg, conf); break;
//        case PMSG_QUEUEUPLOAD:
//                return prpmsg_queuedupload(msg, conf); break;
//        case PMSG_PLACEINQUEUE:
//                return prpmsg_placeinqueue(msg, conf); break;
          case PMSG_UPLOADFAILED:
                  return prpmsg_uploadfailed(msg, conn, conf); break;
//        case PMSG_QUEUEFAILED:
//                return prpmsg_queuefailed(msg, conf); break;
	default:
		return (code);
	}
	}
	return 0;
}

/* takes a message that contains a filelist and creates the structures */
struct cslsk_file *parse_filelist(SLSK_INT nfiles, struct slsk_buf *msg)
{
	struct cslsk_file *files;
	SLSK_INT ifiles, iattr;

	files = calloc(nfiles, sizeof(struct cslsk_file));
	for (ifiles = 0; ifiles < nfiles; ifiles++) {
		files[ifiles].code = msg2byte(msg, msg);
		files[ifiles].filename = msg2str(msg, msg);
		files[ifiles].size1 = msg2int(msg, msg);
		files[ifiles].size2 = msg2int(msg, msg);
		files[ifiles].ext = msg2str(msg, msg);
		files[ifiles].nattr = msg2int(msg, msg);
		files[ifiles].attr = calloc(files[ifiles].nattr,
				sizeof(struct cslsk_file_attr));
		for (iattr = 0; iattr < files[ifiles].nattr; iattr++) {
			files[ifiles].attr[iattr].type = msg2int(msg, msg);
			files[ifiles].attr[iattr].value = msg2int(msg, msg);
		}
	}
	return files;
}

int prpmsg_filesearchresult (struct slsk_buf *msg, struct slsk_peer *peer, struct cslsk_conf *conf)
{
	struct slsk_buf *dst_buf = malloc(sizeof(struct slsk_buf));
	SLSK_INT src_len;

	SLSK_INT token, nfiles;
	struct slsk_result *result;
	struct slsk_search *search;
	int unknownsearch = 1;

	dst_buf->len = 1024*1024;
	dst_buf->buf = malloc(dst_buf->len);

	src_len = msg->len - PLD_OFFSET;
	msg->pos = PLD_OFFSET;
	if (uncompress((Bytef *)dst_buf->buf, (uLongf *) &dst_buf->len, msg->buf + msg->pos, src_len) != Z_OK)
		return PMSG_FILESEARCHRESULT;
	else {
		result = malloc(sizeof(struct slsk_result));
		result->peer = peer;
		result->peer->username = msg2str(dst_buf, dst_buf);
		token = msg2int(dst_buf, dst_buf);

		result->nfiles = msg2int(dst_buf, dst_buf);

		TAILQ_FOREACH(search, &conf->searches, entry) {
			if (token == search->token) {
				unknownsearch = 0;
				if (TAILQ_EMPTY(&search->results)) {
					TAILQ_INIT(&search->results);
					TAILQ_INSERT_HEAD(&search->results, result, entry);
				} else
					TAILQ_INSERT_HEAD(&search->results, result, entry);
				break;
			}
		}

		if (unknownsearch) {
			printf("I have not searched for this! (unknown token)\n");
			search = malloc(sizeof(struct slsk_search));
			search->token = token;
			search->string = NULL;

			if (TAILQ_EMPTY(&conf->searches)) {
				TAILQ_INIT(&conf->searches);
				TAILQ_INSERT_HEAD(&conf->searches, search, entry);
			} else
				TAILQ_INSERT_TAIL(&conf->searches, search, entry);

			TAILQ_INIT(&search->results);
			TAILQ_INSERT_HEAD(&search->results, result, entry);
		}
		
		nfiles = result->nfiles;

		result->files = parse_filelist(nfiles, dst_buf);

		result->peer->nslots = msg2byte(dst_buf, dst_buf);
		result->peer->speed = msg2int(dst_buf, dst_buf);
		result->peer->qlen = msg2int(dst_buf, dst_buf);

/*		printf("username: %s token: %i nfiles: %i slots: %i speed: %i q: %i\n",
				username, token, nfiles, slots, speed, inqueue);*/
	}
	return 0;
}


int prpmsg_userinforeply (struct slsk_buf *msg, struct slsk_peer *peer)
{
	peer->desc = msg2str(msg, msg);
	if (msg2byte(msg, msg))		/* if has pic ... */
		msg2str(msg, msg);		/* ... get pic */
	peer->nupload = msg2int(msg, msg);
	peer->qlen = msg2int(msg, msg);
	peer->nslots = msg2byte(msg, msg);

	return 0;
}

int prpmsg_transferresponse (struct slsk_buf *msg, struct slsk_conn *conn,
		struct cslsk_conf *conf)
{
	SLSK_INT token;
	struct slsk_transfer *ftransfer, *transfer = NULL;
	struct slsk_msg *smsg;

	token = msg2int(msg, msg);

	printf("got transfer response...\n");
	
	TAILQ_FOREACH(ftransfer, &conf->transfers, entry) {
	if (ftransfer->conn == conn)
	/* yep, the transferresponse message correlates to a transfer i know */		
		transfer = ftransfer;		
	}

	if (!transfer) {
		printf("I don't know this transfer! (unknown transfer response)\n");
		return 0;
	}

	transfer->token = token;
	transfer->allowed = msg2byte(msg, msg);
	if (transfer->allowed) {
		printf("Transfer is allowed! opening new connection (F-type)\n");
		transfer->file->size1 = msg2int(msg, msg);
		transfer->conn = addconn(&conf->conns);
		transfer->conn->peer = conn->peer;
		transfer->conn->token = 0;
		transfer->conn->type = 'F';
		transfer->conn->transfer = transfer;
		transfer->conn->rbuf->len = transfer->file->size1;
		transfer->conn->rbuf->buf = malloc(transfer->file->size1);
		transfer->file->stream = NULL;
		slsk_connect(transfer->conn);
		FD_SET(transfer->conn->fd, &conf->writefds);
		msg2q(mksmsg("lbssi", PMSG_PEERINIT, conf->username, "F", transfer->conn->token),
				&transfer->conn->msgs);

		smsg = postmkmsg(mksmsg("iii", transfer->token, 0, 0));
		smsg->msglen = 12;
		smsg->leftbytes = 12;
		basic_msg2q(smsg, &transfer->conn->msgs);
	}
	else {
		printf ("transfer is not allowed\n");
		transfer->reason = msg2str(msg, msg);
	}
	return 0;
}


int prpmsg_uploadfailed (struct slsk_buf *msg, struct slsk_conn *conn, struct cslsk_conf *conf)
{
	char *filename;
	struct slsk_transfer *ftransfer, *transfer = NULL;

	filename = msg2str(msg, msg);

	TAILQ_FOREACH(ftransfer, &conf->transfers, entry) {
	if (!strcmp(ftransfer->file->filename, filename))
	/* yep, the uploadfailed message correlates to a transfer i know */		
		transfer = ftransfer;		
	}

	if (!transfer) {
		printf("I don't know this transfer! (upload failed message)\n");
		return 0;
	}

	transfer->conn->status = transfer->conn->status & ~SLSK_PSTAT_CONNECTED;
	FD_CLR(transfer->conn->fd, &conf->readfds);
	FD_CLR(transfer->conn->fd, &conf->writefds);
	conn_dstr(transfer->conn->fd);
	transfer->conn->fd = 0;
	transfer->status = transfer->status & SLSK_TSTAT_FAILED;
	transfer->reason = "Failed";

	return 0;
}

int prpmsg_sharedfilelist (struct slsk_buf *msg, struct slsk_peer *peer)
{
	struct slsk_buf *dst_buf = malloc(sizeof(struct slsk_buf));
	SLSK_INT ndirs, idirs;
	SLSK_INT src_len;

	dst_buf->len = 1024*1024;
	dst_buf->buf = malloc(dst_buf->len);

	src_len = msg->len - PLD_OFFSET;
	msg->pos = PLD_OFFSET;
	struct cslsk_directory *dirs;

	msg2int(msg, msg);
	if (uncompress((Bytef *)dst_buf->buf, (uLongf *) &dst_buf->len, msg->buf + msg->pos, src_len) != Z_OK)
		return PMSG_SHAREDFILELIST;
	else {
		ndirs = msg2int(dst_buf, dst_buf);
		dirs = calloc(ndirs, sizeof(struct cslsk_directory));

		for (idirs = 0; idirs < ndirs; idirs++) {
			dirs[idirs].name = msg2str(dst_buf, dst_buf);
			dirs[idirs].nfiles = msg2int(dst_buf, dst_buf);
			dirs[idirs].files = parse_filelist(dirs[idirs].nfiles, dst_buf);
		}
	peer->filelist = malloc(sizeof(struct cslsk_filelist));
	peer->filelist->ndirs = ndirs;
	peer->filelist->dirs = dirs;
	}

	return 0;
}

