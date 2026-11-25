/******************************************************************************
 *
 *    msg_lib.c
 *
 *    Copyright (C) 2005  Dreadnought Incorporated
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

#include <stdio.h>
/*
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
*/
#include "SDL_thread.h"
#include "types.h"
#include "buffer.h"
#include "../sock_lib/sock_lib.h"
#include "msg_lib.h"

#define MSG_STOP  (-1)

#define WAIT_TIME (100)

#ifndef STDIN
#define STDIN  (0)
#endif

#ifndef STDOUT
#define STDOUT  (1)
#endif

PRIVATE int fd_in;
PRIVATE int fd_out;

PRIVATE buffer_t b_in;
PRIVATE buffer_t b_out;

PRIVATE bool f_new_data = FALSE;	// New char avail flag
PRIVATE bool f_done = FALSE;	// Are we done yet? flag
PRIVATE bool f_exit = FALSE;	// exit recvd flag

PRIVATE bool b_server = FALSE;
PRIVATE bool b_using_socket = FALSE;

PRIVATE msg_num_t msg_num;

PRIVATE SOCKET s = -1;

PRIVATE SDL_Thread *thr2;
PRIVATE SDL_Thread *thr3;

PRIVATE int read_incoming(void *unused)
{
	while (!f_done) {
		int cc;

		reset_buf(&b_in);

		read(fd_in, &cc, sizeof(int));

		read(fd_in, b_in.data, ntohl(cc));

		b_in.index = ntohl(cc);

		if (b_in.index <= 0) {
			fprintf(stderr,
				"read_incoming(): *** data in error ***\n");
		}

		msg_num = msg_unpack_int32();

		if (MSG_STOP == msg_num) {
			if (b_server) {
				f_done = TRUE;
			} else {
				f_exit = TRUE;	// client sends msg back
			}
		} else {
			if (!f_done) {	// read() blocks
				do_data_in();
			}
		}
	}

	SDL_WaitThread(thr2, NULL);

	return (0);
}

PRIVATE int write_outgoing(void *unused)
{
	while (!f_done) {

		if (f_new_data) {
			int len;

			len = htonl(b_out.index);

			write(fd_out, &len, sizeof(int));

			write(fd_out, b_out.data, b_out.index);

			f_new_data = FALSE;
		}

		usleep(WAIT_TIME);
	}

	SDL_WaitThread(thr3, NULL);

	return (0);
}

//////////////////////////////////////////////////////////////////////////////
//
//                      PUBLIC PROCEDURES BELOW
//
//////////////////////////////////////////////////////////////////////////////

PUBLIC int msg_init_inetd_server(void)
{
	fd_in = STDIN;
	fd_out = STDOUT;

	b_server = TRUE;

	f_done = FALSE;

	thr2 = SDL_CreateThread(read_incoming, NULL);
	if (thr2 == NULL) {
		fprintf(stderr,
			"msg_init_server(): SDL_CreateThread() failed\n");
		return (-1);
	}

	thr3 = SDL_CreateThread(write_outgoing, NULL);
	if (thr3 == NULL) {
		fprintf(stderr,
			"msg_init_server(): SDL_CreateThread() failed\n");
		return (-1);
	}

	return (0);
}

PUBLIC int msg_init_tcp_server(int port)
{
	s = init_tcp_server(port);

	if (s < 0) {
		return (-1);
	}

	fd_in = s;
	fd_out = s;

	b_server = TRUE;

	f_done = FALSE;

	thr2 = SDL_CreateThread(read_incoming, NULL);
	if (thr2 == NULL) {
		fprintf(stderr,
			"msg_init_tcp_server(): SDL_CreateThread() failed\n");
		return (-1);
	}

	thr3 = SDL_CreateThread(write_outgoing, NULL);
	if (thr3 == NULL) {
		fprintf(stderr,
			"msg_init_tcp_server(): SDL_CreateThread() failed\n");
		return (-1);
	}

	return (0);
}

PUBLIC int msg_init_tcp_client(const char *server, int port)
{
	s = init_tcp_client(server, port);

	if (s < 0) {
		return (-1);
	}

	b_using_socket = TRUE;

	fd_in = s;
	fd_out = s;

	f_done = FALSE;
	f_exit = FALSE;

	thr2 = SDL_CreateThread(read_incoming, NULL);
	if (thr2 == NULL) {
		fprintf(stderr,
			"msg_init_tcp_client(): SDL_CreateThread() failed\n");
		return (-1);
	}

	thr3 = SDL_CreateThread(write_outgoing, NULL);
	if (thr3 == NULL) {
		fprintf(stderr,
			"msg_init_tcp_client(): SDL_CreateThread() failed\n");
		return (-1);
	}

	return (0);
}

PUBLIC int msg_init_client(int in, int out)
{
	fd_in = in;
	fd_out = out;

	f_done = FALSE;
	f_exit = FALSE;

	thr2 = SDL_CreateThread(read_incoming, NULL);
	if (thr2 == NULL) {
		fprintf(stderr,
			"msg_init_client(): SDL_CreateThread() failed\n");
		return (-1);
	}

	thr3 = SDL_CreateThread(write_outgoing, NULL);
	if (thr3 == NULL) {
		fprintf(stderr,
			"msg_init_client(): SDL_CreateThread() failed\n");
		return (-1);
	}

	return (0);
}

PUBLIC void msg_stop_server(void)
{
	msg_set_num(MSG_STOP);
	msg_send();

	// wait until exit message returns
	while (!f_done)
		usleep(WAIT_TIME);

	usleep(WAIT_TIME);	// Give time for client to shutdown

	if (s >= 0) {
		close_socket(s);
	}
//      SDL_WaitThread(thr3, NULL);
//      SDL_WaitThread(thr2, NULL);
}

PUBLIC void msg_stop_client(void)
{
//      printf("msg_stop_client(): waiting for MSG_STOP\n");
//      fflush(stdout);

	while (!f_exit) {
		usleep(WAIT_TIME);
	}


//      printf("msg_stop_client(): sending MSG_STOP\n");
//      fflush(stdout);

	msg_set_num(MSG_STOP);
	msg_send();

//      printf("msg_stop_client(): make sure MSG_STOP sent\n");
//      fflush(stdout);

	// make sure exit msg sent
	while (f_new_data) {
		usleep(WAIT_TIME);
	}

//      printf("msg_stop_client(): setting f_done = TRUE.\n");
//      fflush(stdout);

	f_done = TRUE;

	if (b_using_socket) {
		close_socket(s);
	}
//      printf("msg_stop_client(): exiting.\n");
//      fflush(stdout);

//      pthread_exit(NULL);
}

PUBLIC void msg_send(void)
{
	f_new_data = TRUE;
}

PUBLIC void msg_set_num(msg_num_t mn)
{
	// wait until writer is ready to write
	while (f_new_data)
		usleep(WAIT_TIME);

	reset_buf(&b_out);

	msg_pack_int32(mn);
}

PUBLIC msg_num_t msg_get_num(void)
{
	return (msg_num);
}

PUBLIC void msg_pack_string(const char *s)
{
	pack_string(&b_out, s);
}

PUBLIC void msg_unpack_string(char *s)
{
	unpack_string(&b_in, s);
}

PUBLIC void msg_pack_int32(int32 i32)
{
	pack_int32(&b_out, i32);
}

PUBLIC int32 msg_unpack_int32(void)
{
	return (unpack_int32(&b_in));
}
