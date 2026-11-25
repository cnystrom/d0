/******************************************************************************
 *
 *    scp_lib.c
 *
 *    Copyright (C) 2004, 2005  Chris Nystrom
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
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>
#include <pty.h>		// Linux
//#include <util.h>    // OS X
#include <sys/ioctl.h>
#include "uio.h"
//#include "wlog.h"
#include "scp_lib.h"

PRIVATE int call_scp(const char *from_file, const char *to_file);

PRIVATE int read_scp(byte * obuf, int count, int uio);
PRIVATE int write_scp(const byte * buf);

PRIVATE void display_scp(void);

PRIVATE int verify_scp_passwd(const char *passwd);
PRIVATE int scp_passwd_prompt(const char *buf);
PRIVATE int check_scp_login(const char *buf);

PRIVATE void done_scp(void);

#ifndef NEWLINE
#define NEWLINE  (10);
#endif

PRIVATE int get_scp_master(void);
PRIVATE void get_scp_slave(void);
PRIVATE void set_scp_tty(void);
PRIVATE void scp_fail(void);

PRIVATE int scp_master;
PRIVATE int scp_slave;
PRIVATE int scp_child;

PRIVATE struct termios scp_tt;

PUBLIC int
scp_file(const char *from_file, const char *to_file, const char *passwd)
{
	int ret;

//  printf("scp_file(): from_file = |%s|\n", from_file);
//  printf("scp_file(): to_file = |%s|\n", to_file);
//  printf("scp_file(): passwd = |%s|\n", passwd);

	ret = call_scp(from_file, to_file);

//  printf("scp_file(): call_scp() ret = %d\n", ret);

	if (ret < 0) {
		printf("scp call failed\n");
		return (-1);
	}

	ret = verify_scp_passwd(passwd);

//  printf("scp_file(): verify_scp_passwd() ret = %d\n", ret);

	if (ret < 0) {
		printf("login or password bad.\n");
		return (-2);
	}

	display_scp();

	return (0);
}

PRIVATE int call_scp(const char *from_file, const char *to_file)
{
	get_scp_master();
	set_scp_tty();
	scp_child = fork();

	if (scp_child < 0) {
		perror("fork");
		scp_fail();
	}

	if (scp_child == 0) {
		get_scp_slave();
		close(scp_master);
		dup2(scp_slave, STDIN_FILENO);
		dup2(scp_slave, STDOUT_FILENO);
		dup2(scp_slave, STDERR_FILENO);
		close(scp_slave);
		execl("/usr/bin/scp", "/usr/bin/scp", "-q", from_file,
		      to_file, (char *) 0);
		perror("execl failed");
		scp_fail();
		return 0;
	}

	close(scp_slave);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &scp_tt);
	return (scp_master);
}

PRIVATE int get_scp_master(void)
{
	int ret;
	struct winsize win;

	tcgetattr(STDIN_FILENO, &scp_tt);
	ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &win);

	ret = openpty(&scp_master, &scp_slave, NULL, &scp_tt, &win);

	if (ret < 0) {
		fprintf(stderr, "openpty failed\n");
//    wlog("openpty failed\n");
		scp_fail();
	}

	return (ret);
}

PRIVATE void get_scp_slave(void)
{
	setsid();
	ioctl(scp_slave, TIOCSCTTY, 0);
}

PRIVATE void set_scp_tty(void)
{
	struct termios rtt;

	rtt = scp_tt;
	cfmakeraw(&rtt);
	rtt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &rtt);
}

PRIVATE void scp_fail(void)
{
	kill(0, SIGTERM);
	done_scp();
}

PRIVATE void done_scp(void)
{
	if (scp_child) {
		close(scp_master);
	} else {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &scp_tt);
	}
}

PRIVATE int read_scp(byte * buf, int count, int uio)
{
	int ret;

//  wlog("--\nentering read_scp()...\n");

	if (uio) {
//    wlog("using uio.\n");
		ret = uio_get_line(scp_master, buf, count);
	} else {
//    wlog("not using uio.\n");
		ret = read(scp_master, buf, count);
	}

	if (ret < 0) {
		*buf = '\0';
	} else {
		*(buf + ret) = '\0';
	}

//  wlog("read_scp(): buf = |%s|\n\n", buf);

//  wlog("read_scp(): ret = %d\n", ret);
//  wlog("exiting read_scp()...\n\n");

	return (ret);
}

PRIVATE int write_scp(const byte * ibuf)
{
	int i;
	int count;
	int ret;
	byte tempBuf[BUFSIZ];

//  wlog("write_scp(): ibuf = |%s|\n", (char *) ibuf);

	count = strlen(ibuf);

	if (count > (BUFSIZ - 1)) {
		count = (BUFSIZ - 1);
//    wlog("count adjusted.\n");
	}
//  wlog("--\nwrite_scp(): count = %d\n", count);

	for (i = 0; i < count; i++) {
		tempBuf[i] = *(ibuf + i);
	}

	tempBuf[i++] = NEWLINE;
	count++;

	tempBuf[i] = 0;

//  wlog("write_scp(): tempBuf = |%s|\n", tempBuf);

	ret = uio_put_line(scp_master, tempBuf);

//  ret = write(scp_master, tempBuf, (count + 2));

//  wlog("count = %d\n", count);
//  wlog("ret   = %d\n", ret);

	if (ret != count) {
//    wlog("write_scp(): write error.\n");
//    wlog("(count + 2) = %d\n", (count + 2));
//    wlog("ret = %d\n", ret);
		fprintf(stderr, "write_scp(): write error.\n");
		fprintf(stderr, "write_scp(): count = %d.\n", count);
		fprintf(stderr, "write_scp(): ret = %d.\n", ret);
		fprintf(stderr, "write_scp(): ibuf = |%s|.\n", ibuf);
		fprintf(stderr, "write_scp(): tempBuf = |%s|.\n", tempBuf);
	}
//  wlog("--\n");

	return (ret);
}

PRIVATE void display_scp(void)
{
	int cc;			// char count
	int flag = 1;		// flag for login check
	int ret;
	char buf[BUFSIZ];

	cc = read_scp(buf, BUFSIZ, USE_UIO);

	while (cc > 0) {

		if (flag) {
			ret = check_scp_login(buf);

			if (ret == LOGIN_BAD) {
				printf("error(): login bad.\n");
				done_scp();
				return;
			} else if (ret == NO_PROG) {
				printf("error(): program missing.\n");
				done_scp();
				return;
			} else {
				flag = 0;
			}
		}

		printf(buf);
		cc = read_scp(buf, BUFSIZ, USE_UIO);
	}

	done_scp();
}

PRIVATE int verify_scp_passwd(const char *passwd)
{
	int cc;			// char count
	char buf[BUFSIZ];

//  wlog("passwd = %s\n", passwd);

	cc = read_scp(buf, BUFSIZ, USE_READ);

//  wlog("ssh cc # = %d\n", cc);

	while (cc > 0) {

		buf[cc] = 0;

//    wlog("buf = |%s|\n", buf);

		if (scp_passwd_prompt(buf)) {
//      wlog("password prompt recieved.\n");
			write_scp(passwd);
//      wlog("password sent.\n");
			return 0;
		}

		cc = read_scp(buf, BUFSIZ, USE_READ);
	}

//  wlog("password verification failed\n");
	done_scp();
	return (-1);
}

PRIVATE int scp_passwd_prompt(const char *s)
{
	int len;

	len = strlen(s);
//  wlog("len = %d\n", len);

	if (len <= 9) {
//    wlog("-= no password match =- 1\n");
		return 0;
	}

	if (strncmp(s + len - 9, "assword: ", 9) == 0) {
//    wlog("-= password prompt =-\n");
		return 1;
	} else {
//    wlog("-= not password prompt =- 2\n");
		return 0;
	}
}

PRIVATE int check_scp_login(const char *buf)
{
//  wlog("passwd_failed(): buf = |%s|\n", buf);

	if (strncmp(buf, "Permission denied", 17) == 0) {
//    wlog("password failed\n");
		return LOGIN_BAD;
	} else if (strncmp(buf, "bash", 4) == 0) {
//    wlog("program not there.\n");
		return NO_PROG;
	} else {
//    wlog("password ok\n");
		return 0;
	}
}
