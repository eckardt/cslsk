/*      $Id: commands.h,v 1.7 2005/02/09 17:10:01 se Exp $ */

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


/*
 * define the commands as you like...
 * i pretty much like the "ed" approach using very short command names,
 * so you don't have to type so many letters ;)
 */

//#define CMD_CONNECT			"connect"
#define CMD_LOGIN			"li"
#define CMD_LOGOUT			"lo"
#define CMD_ROOMLIST			"rl"
#define CMD_UPDATEROOMLIST		"url"
//#define CMD_DISCONNECT		"disc"
#define CMD_EXIT			"x"
#define CMD_JOINROOM			"j"
#define CMD_LEAVEROOM			"lv"
#define CMD_SAYCHATROOM			"s"
#define CMD_STATUS			"st"
#define CMD_PEERCONN			"pc"
#define CMD_SEARCH			"sr"
#define CMD_LISTSEARCHES		"ls"
#define CMD_LISTTRANSFERS		"lt"
#define CMD_DOWNLOAD			"d"
#define CMD_GETFILELIST			"gfl"
#define CMD_SHOWFILELIST		"sfl"
