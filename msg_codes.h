/*      $Id: msg_codes.h,v 1.7 2005/01/31 09:26:03 se Exp $ */

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


#define CLIENT_VERSION			155

#define MSG_LOGIN			1
#define MSG_SETWAITPORT			2
#define MSG_GETPEERADDRESS		3
#define MSG_ADDUSER			5
#define MSG_UNKNOWN1			6
#define MSG_GETUSERSTATUS		7
#define MSG_SAYCHATROOM			13
#define MSG_JOINROOM			14
#define MSG_LEAVEROOM			15
#define MSG_USERJOINEDROOM		16
#define MSG_USERLEFTROOM		17
#define MSG_CONNECTTOPEER		18
#define MSG_MESSAGEUSER			22
#define MSG_MESSAGEACKED		23
#define MSG_FILESEARCH			26
#define MSG_SETSTATUS			28
#define MSG_SERVERPING			32
#define MSG_SENDSPEED			34
#define MSG_SHAREDFOLDERSFILES		35
#define MSG_GETUSERSTATS		36
#define MSG_QUEUEDDOWNLOADS		40
#define MSG_RELOGGED			41
#define MSG_PLACEINLINERESPONSE		60
#define MSG_ROOMADDED			62
#define MSG_ROOMREMOVED			63
#define MSG_ROOMLIST			64
#define MSG_EXACTFILESEARCH		65
#define MSG_ADMINMESSAGE		66
#define MSG_GLOBALUSERLIST		67
#define MSG_TUNNELEDMESSAGE		68
#define MSG_PRIVILEGEDUSERS		69
#define MSG_GETPARENTLIST		71
#define MSG_UNKNOWN2			73
#define MSG_PARENTINACTIVITYTIMEOUT	86
#define MSG_SEARCHINACTIVITYTIMEOUT	87
#define MSG_MINPARENTSINCACHE		88
#define MSG_DISTRIBALIVEINTERVAL	90
#define MSG_ADDTOPRIVILEGED		91
#define MSG_CHECKPRIVILEGES		92
#define MSG_CANTCONNECTTOPEER		1001

#define MSG_HAVENOPARENT		71
#define MSG_SEARCHREQUEST		93
#define MSG_NETINFO			102
#define MSG_WISHLISTSEARCH		103
#define MSG_WISHLISTINTERVAL		104
#define MSG_ROOMTICKERSTATE		113
#define MSG_ROOMTICKERADD		114
#define MSG_ROOMTICKERREMOVE		115
#define MSG_ROOMTICKERSET		116
#define MSG_REMOVEUSER			324534 //???
#define MSG_CANTCREATEROOM		367567 //???

#define PMSG_PEERINIT			1	/* this is a byte */
#define PMSG_PIERCEFW			0	/* this is a byte */

#define PMSG_GETSHAREDFILELIST		4
#define PMSG_SHAREDFILELIST		5
#define PMSG_FILESEARCHREQUEST		8
#define PMSG_FILESEARCHRESULT		9
#define PMSG_USERINFOREQUEST		15
#define PMSG_USERINFOREPLY		16
#define PMSG_FOLDERCONTENTSREQUEST	36
#define PMSG_FOLDERCONTENTSRESPONSE	37
#define PMSG_TRANSFERREQUEST		40
#define PMSG_TRANSFERRESPONSE		41
#define PMSG_PLACEHOLDUPLOAD		42
#define PMSG_QUEUEUPLOAD		43
#define PMSG_PLACEINQUEUE		44
#define PMSG_UPLOADFAILED		46
#define PMSG_QUEUEFAILED		50
#define PMSG_PLACEINQUEUEREQUEST	51

#define PCONNTYPE_DISTRIBUTEDSEARCH	"D"
#define PCONNTYPE_FILETRANSFER		"F"