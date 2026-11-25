/******************************************************************************
 *
 *    buffer.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "types.h"
#include "buffer.h"

#define BUFF_ERR1 "buffer overflow"

void reset_buf(buffer_ptr b)
{
	b->index = 0;
	b->read_index = 0;
	b->data[0] = 0;
}

/*
void display_buf(buffer_ptr b)
{
	int i;

	wlog("\n----\n");
	wlog("buffer =\n");
	for (i = 0; i < b->index; i++) {
		wlog("buf[%d] = %d ", i, b->data[i]);

		if (b->data[i] > 32) {
			wlog("%c\n", b->data[i]);
		} else {
			wlog("\n");
		}
	}
	wlog("----\n\n");
}
*/

void pack_int8(buffer_ptr b, int8 in)
{
	if ((b->index + INT8_SIZE) >= BUF_SIZE) {
		fprintf(stderr, "pack_int8(): %s\n", BUFF_ERR1);
		return;
	}

	b->data[b->index++] = in;
}

int8 unpack_int8(buffer_ptr b)
{
	if ((b->read_index + INT8_SIZE) > b->index) {
		fprintf(stderr, "unpack_int8(): %s\n", BUFF_ERR1);
		return (-1);
	}

	return (b->data[b->read_index++]);
}

void pack_int16(buffer_ptr b, int16 in)
{
	int i;
	byte *bp;
	int16 tmp;

//  printf("pack_int16(): b.index = %d\n", b.index);
//  printf("pack_int16(): b.read_index = %d\n", b.read_index);

	if ((b->index + INT16_SIZE) >= BUF_SIZE) {
		fprintf(stderr, "pack_int16(): %s\n", BUFF_ERR1);
		return;
	}

	tmp = htons(in);
	bp = (byte *) & tmp;

	for (i = 0; i < INT16_SIZE; i++) {
		b->data[b->index++] = *(bp++);
	}
}

int16 unpack_int16(buffer_ptr b)
{
	int i;
	byte *bp;
	int16 out;

//      printf("unpack_int16(): b->index = %d\n", b->index);
//      printf("unpack_int16(): b->read_index = %d\n", b->read_index);

	if ((b->read_index + INT16_SIZE) > b->index) {
		fprintf(stderr, "unpack_int16(): %s\n", BUFF_ERR1);
		return (-1);
	}

	bp = (byte *) & out;

	for (i = 0; i < INT16_SIZE; i++) {
		*(bp++) = b->data[b->read_index++];
	}

//      printf("unpack_int16(): int16 = %d\n", int16);

	return (ntohs(out));
}

void pack_int32(buffer_ptr b, int32 in)
{
	int i;
	byte *bp;
	int32 tmp;

	if ((b->index + INT32_SIZE) >= BUF_SIZE) {
		fprintf(stderr, "pack_int32(): %s\n", BUFF_ERR1);
		return;
	}

	tmp = htonl(in);

	bp = (byte *) & tmp;

	for (i = 0; i < INT32_SIZE; i++) {
		b->data[b->index++] = *(bp++);
	}
}

int32 unpack_int32(buffer_ptr b)
{
	int i;
	byte *bp;
	int32 out;

	if ((b->read_index + INT32_SIZE) > b->index) {
		fprintf(stderr, "unpack_int32(): %s\n", BUFF_ERR1);
		return (-1);
	}

	bp = (byte *) & out;

	for (i = 0; i < INT32_SIZE; i++) {
		*(bp++) = b->data[b->read_index++];
	}


	return (ntohl(out));
}

void pack_string(buffer_ptr b, const char *s)
{
	int i;
	uint32 len;
	byte *bp;

	len = strlen(s) + 1;	// Add the NUL termination

	if ((b->index + INT32_SIZE + len) >= BUF_SIZE) {
		fprintf(stderr, "pack_string(): %s\n", BUFF_ERR1);
		return;
	}

	pack_int32(b, len);

	bp = (byte *) s;

	for (i = 0; i < len; i++) {
		b->data[b->index++] = *(bp++);
	}
}

void unpack_string(buffer_ptr b, char *s)
{
	int i;
	uint32 len;
	byte *bp;

	if ((b->read_index + INT32_SIZE) > b->index) {
		fprintf(stderr, "unpack_string(): %s\n", BUFF_ERR1);
		return;
	}

	len = unpack_int32(b);

	if ((b->read_index + len) > b->index) {
		fprintf(stderr, "unpack_string(): %s\n", BUFF_ERR1);
		return;
	}

	bp = (byte *) s;

	for (i = 0; i < len; i++) {
		*(bp++) = b->data[b->read_index++];
	}

	return;
}

bool cmp_buf(buffer_ptr b, char *s)
{
	int len;

	len = strlen(s);

	if (strncmp(b->data, s, len) == 0) {
		return (TRUE);
	} else {
		return (FALSE);
	}
}

/*
void copy_buf(buffer_ptr b_in, buffer_ptr b_out)
{
	reset_buf(b_out);

	strlcpy(b_out->data, b_in->data, b_in->index);

	b_out->index = b_in->index;
}
*/
/*
void pack_buffer(buffer_ptr b)
{
	int8 v1 = CHAR_MAX;
	int16 w1 = SHRT_MAX;
	int32 x1 = INT_MAX;
	char s1[] = "Test";

	reset_buf(b);

	pack_int8(b, v1);
	pack_int16(b, w1);
	pack_int32(b, x1);
	pack_string(b, s1);
}

void unpack_buffer(buffer_ptr b)
{
	int8 v2 = 0;
	int16 w2 = 0;
	int32 x2 = 0;
	char s2[16] = "";

	v2 = unpack_int8(b);
	w2 = unpack_int16(b);
	x2 = unpack_int32(b);
	unpack_string(b, s2);

	wlog("v2 = %d\n", v2);
	wlog("w2 = %d\n", w2);
	wlog("x2 = %d\n", x2);
	wlog("s2 = |%s|\n\n", s2);
}

#ifndef STDOUT
#define STDOUT  (1)
#endif

int main(void)
{
	wlog_open_fd(STDOUT);

	buffer_t b;

	pack_buffer(&b);

	display_buf(&b);

	unpack_buffer(&b);

	wlog_close();

	exit(0);
}
*/
