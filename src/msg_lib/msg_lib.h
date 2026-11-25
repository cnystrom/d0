/******************************************************************************
 *
 *    msg_lib.h
 *
 *    Copyright (C) 2005  Chris Nystrom
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU Lessor General Public License as published
 *    by the Free Software Foundation; either version 2.1 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but without any warranty whatsoever.
 *
 *    For more license information see:
 *
 *      http://www.gnu.org/copyleft/lessor.html
 *
 *    Contact Author at:
 *
 *    Chris Nystrom
 *
 *    E-Mail: cnystrom@gmail.com
 *    Blog:   http://conversazione.blogspot.com
 *    AIM:    nystromchris
 *
 *    Soli Deo Gloria
 *
 */

#ifndef _MSG_LIB_H
#define _MSG_LIB_H

#ifndef _TYPES_H
#include "types.h"
#endif

typedef sint32 msg_num_t;

extern void do_data_in(void);

int msg_init_inetd_server(void);
int msg_init_tcp_server(int port);
int msg_init_tcp_client(const char *server, int port);
int msg_init_client(int in, int out);

void msg_stop_server(void);
void msg_stop_client(void);

void msg_set_num(msg_num_t mn);
msg_num_t msg_get_num(void);

void msg_pack_string(const char *s);
void msg_unpack_string(char *s);

void msg_pack_int32(int32 i32);
int32 msg_unpack_int32(void);

void msg_send(void);

#endif				/* _MSG_LIB_H */
