/*      $Id: mksmsg.h,v 1.5 2005/02/09 17:10:01 se Exp $ */

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


#ifndef __CSLSK_MKSMSG_H
#define __CSLSK_MKSMSG_H

struct slsk_buf *mksmsg(char *fmt, ...);
/*
 * login: 		"lissisi" -> str username, str pwd, int version, str id, int 216
 * 	logging into the server
 * setwaitport:		"lii" -> int port
 * 	telling the server what port we are listening on
 * getuserstats		"lis" -> str username
 * getpeeraddress	"lis" -> str username
 * 	asking for the peers address given the username
 * adduser		"lis" -> str username
 * 	start watching the online status for a user
 * getuserstatus	"lis" -> str username
 * 	request a users current status (online/away/offline...)
 * saychatroom		"liss" -> str room, str room_msg
 * joinroom		"lis" -> str room
 * leaveroom		"lis" -> str room
 * connecttopeer	"liiss" -> int token, str username, str type
 * 	telling the server to tell the peer to contact us back (peer is behind firewall)
 * messageuser		"liss" -> str username, str user_msg
 * messageacked		"lii" -> int privmsgid
 * filesearch		"liis" -> int token, str search_text
 * setstatus		"lii" -> int new_status
 * sharedfoldersfiles	"liii" -> int nfolders, int nfiles
 * getparentlist	"lib" -> byte b
 * 	
 * placeinlineresponse	"lisii" -> str username, int token, int place
 * roomlist		"li"
 * globaluserlist	"li"
 * tunneledmessage	"lisiiiis" -> str username, int code, int token, int, int, str message
 * privilegedusers	"li"
 * cantconnecttopeer	"liis" -> int token, str username
 * 	telling the server that we cannot connect to a peer
 */

/*
 * peerinit:		"lbssii" -> BYTE(!) code, str username, str type, int 300, int token
 * piercefw:		"lbi" -> BYTE(!) code, int token
 * getsharedfilelist	"li"
 * userinforequest	"li"
 * userinforeply	"lisbSiiii" -> str description, byte picflag, Str filecontent, int uploads,
 * 					int downloads, int qlen, int nslots
 * transferrequest	"liiisi" -> int dir-constant, int token, str filename, int filesize
 * ... */

char * mk_smsghdr(SLSK_INT len, SLSK_INT code);
#define MSG_SIZE 1024
#endif /* __CSLSK_MKSMSG_H */
