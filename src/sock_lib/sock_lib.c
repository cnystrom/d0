/******************************************************************************
 *
 *    sock_lib.c
 *
 *    Copyright (C) 2002, 2005  Chris Nystrom
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

#include "sock_lib.h"

unsigned short pn;		/* port number */

SOCKET establish_socket(unsigned short portnum)
{
	int reuse_addr = 1;
	char myname[256];
	struct sockaddr_in sa;
	struct hostent *hp;
	SOCKET s;

	memset(&sa, 0, sizeof(struct sockaddr_in));

	gethostname(myname, sizeof(myname));

	hp = gethostbyname(myname);

	if (hp == NULL)
		return (INVALID_SOCKET);

	sa.sin_family = hp->h_addrtype;

	sa.sin_port = htons(portnum);

	s = socket(AF_INET, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET)
		return INVALID_SOCKET;

	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
		   sizeof(reuse_addr));

	if (bind(s, (struct sockaddr *) &sa, sizeof(struct sockaddr_in))
#ifdef WIN32
	    == SOCKET_ERROR)
#else
	    < 0)
#endif
	{
#ifdef WIN32
		closesocket(s);
#else
		close(s);
#endif

		return (INVALID_SOCKET);
	}

	listen(s, 3);		// max # of queued connects 
	return (s);
}

SOCKET open_socket(unsigned short portnum)
{
	SOCKET s;

	pn = portnum;

	if ((s = establish_socket(portnum)) == INVALID_SOCKET) {
		perror("establish");
		exit(1);
	}

	return (s);
}

SOCKET accept_socket(SOCKET s)
{
	SOCKET new_sock;

	new_sock = accept(s, NULL, NULL);

	if (s == INVALID_SOCKET) {
		fprintf(stderr, "Error waiting for new connection!\n");
		return (INVALID_SOCKET);
	}

	return (new_sock);
}

void close_socket(SOCKET s)
{
#ifdef WIN32
	closesocket(s);
#else
	close(s);
#endif
}

void puts_socket(SOCKET s, char *str)
{
	int i;

	i = strlen(str);

//      printf("OUT> %s\n", str);

	if (send(s, str, i, 0) == -1)
		perror("send");
}

u_int gets_socket(SOCKET s, char *str, u_int max_len)
{
	int i;

	if ((i = recv(s, str, max_len - 1, 0)) == -1)
		perror("recv");

	str[i] = '\0';

//      printf("IN> %s\n", str);

	return (strlen(str));
}

SOCKET init_tcp_server(int port_num)
{
	SOCKET s, s_tmp;

	s_tmp = open_socket(port_num);

	s = accept_socket(s_tmp);

	return (s);
}

SOCKET init_tcp_client(const char *hostname, unsigned short portnum)
{
	struct sockaddr_in sa;
	struct hostent *hp;
	SOCKET s;

	hp = gethostbyname(hostname);

	if (hp == NULL)
		return INVALID_SOCKET;

	memset(&sa, 0, sizeof(sa));

	memcpy((char *) &sa.sin_addr, hp->h_addr, hp->h_length);

	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons((u_short) portnum);

	s = socket(hp->h_addrtype, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET)
		return INVALID_SOCKET;

	if (connect(s, (struct sockaddr *) &sa, sizeof sa)
#ifdef WIN32
	    == SOCKET_ERROR)
#else
	    < 0)
#endif
	{
		close_socket(s);
		return INVALID_SOCKET;
	}

	return s;
}

int read_socket(SOCKET s, char *buf, int n)
{
	return (read(s, buf, n));
}

int read_socket_n(SOCKET s, char *buf, int n)
{
	int bcount;		/* counts bytes read */
	int br;			/* bytes read this pass */

	bcount = 0;
	br = 0;

	while (bcount < n) {	// loop until full buffer 
		printf("bcount = %d\n", bcount);
		if ((br = read(s, buf, n - bcount)) > 0) {
			bcount += br;	// increment byte counter 
			buf += br;	// move buffer ptr for next read 
		} else if (br < 0)	// signal an error to the caller 
			return (-1);
	}

	return (bcount);
}

int write_socket(SOCKET s, char *buf, int n)
{
	int nleft, nwritten;

	nleft = n;

	while (nleft > 0) {
		nwritten = write(s, buf, nleft);

		if (nwritten <= 0)
			return (nwritten);	// error 

		nleft -= nwritten;
		buf += nwritten;
	}

	return (n - nleft);
}

PRIVATE socklen_t addrlen;

PRIVATE void init_udp_socket(udp_sock_ptr s)
{
	s->sockid = -1;
	addrlen = sizeof(struct sockaddr_in);
	bzero((char *) &s->local_addr, addrlen);
	bzero((char *) &s->remote_addr, addrlen);
	s->connected = FALSE;
}

PUBLIC int init_udp_server(udp_sock_ptr s, int port)
{
	init_udp_socket(s);

	if ((s->sockid = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "init_udp_server(): socket error: %d\n",
			errno);
		return (-1);
	}

	s->local_addr.sin_family = AF_INET;
	s->local_addr.sin_addr.s_addr = htons(INADDR_ANY);
	s->local_addr.sin_port = htons(port);

	if ((bind(s->sockid, (struct sockaddr *) &s->local_addr, addrlen) <
	     0)) {
		fprintf(stderr, "init_udp_server(): bind failed: %d\n",
			errno);
		return (-2);
	}

	return (s->sockid);
}

/* inetd handles most of setup */
PUBLIC int init_udp_inetd_server(udp_sock_ptr s)
{
	init_udp_socket(s);

	s->sockid = 0;

	return (s->sockid);
}

PUBLIC int init_udp_client(udp_sock_ptr s, const char *hostname, int port)
{
	int ret;
	struct hostent *hp;

	hp = gethostbyname(hostname);

	if (hp == NULL) {
		return (-1);
	}
	init_udp_socket(s);

	memcpy((char *) &s->remote_addr.sin_addr, hp->h_addr,
	       hp->h_length);
	s->remote_addr.sin_family = hp->h_addrtype;
	s->remote_addr.sin_port = htons(port);

	if ((s->sockid = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "init_udp_client(): socket failed: %d\n",
			errno);
		return (-2);
	}

	s->local_addr.sin_family = AF_INET;
	s->local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s->local_addr.sin_port = htons(0);

	if ((bind(s->sockid, (struct sockaddr *) &s->local_addr, addrlen) <
	     0)) {
		fprintf(stderr, "init_udp_client(): bind failed: %d\n",
			errno);
		return (-3);
	}

	/* client knows who to talk to */
	ret = connect_udp(s);

	if (ret < 0) {
		return (-4);
	}

	return (s->sockid);
}

/* must call after read_udp() on server so server knows who to connect to */
PUBLIC int connect_udp(udp_sock_ptr s)
{
	int ret;

	ret =
	    connect(s->sockid, (struct sockaddr *) &s->remote_addr,
		    addrlen);

	if (ret < 0) {
		fprintf(stderr, "connect_udp(): connect failed: %d\n",
			ret);
		return (-1);
	} else {
		s->connected = TRUE;
	}

	return (ret);
}

PUBLIC int read_udp(udp_sock_ptr s, byte_ptr msg, int len)
{
	int cc;

	/* note: this call fills in the remote address info */
	cc = recvfrom(s->sockid, msg, len, 0,
		      (struct sockaddr *) &s->remote_addr, &addrlen);

	usleep(0);		// strangly this seems to help...

	return (cc);
}

PUBLIC int write_udp(udp_sock_ptr s, const byte_ptr msg, int len)
{
	int retcode;

	retcode =
	    sendto(s->sockid, msg, len, 0,
		   (struct sockaddr *) &s->remote_addr, addrlen);

	if (retcode <= -1) {
		fprintf(stderr, "write_udp(): sendto failed: %d\n", errno);
		return (-1);
	}

	return (retcode);
}

PUBLIC void close_udp(udp_sock_ptr s)
{
	close(s->sockid);
}
