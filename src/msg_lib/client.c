/******************************************************************************
 *
 *    client.h
 *
 *    Copyright (C) 2005  Chris Nystrom
 *
 *    This program is public domain software
 *
 *    This program is distributed in the hope that it will be useful,
 *    but without any warranty whatsoever.
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
#include <stdlib.h>
#include "msg_lib.h"

#define PORT_NUM (32664)

#define SEND_STRING (0)

void do_data_in(void)
{
	msg_num_t mn;
	char string[12];

	mn = msg_get_num();

	if (SEND_STRING == mn) {
		msg_unpack_string(string);

		printf("IN> |%s|\n", string);
		fflush(stdout);
	} else {
		fprintf(stderr, "do_data_in(): bad function number: %d\n",
			mn);
	}
}

void test_write(const char *s)
{
	msg_set_num(SEND_STRING);	// msg_num
	msg_pack_string(s);
	msg_send();

	printf("OUT> |%s|\n", s);
	fflush(stdout);
}

int main(void)
{
	int i;
	char string[12];

	msg_init_tcp_client("localhost", PORT_NUM);

	for (i = 0; i < 10000; i++) {
		sprintf(string, "Test C%d", i);
		test_write(string);
	}

	msg_stop_client();

	exit(0);
}
