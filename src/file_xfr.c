/******************************************************************************
 *
 *    file_xfr.c
 *
 *    Copyright (C) 2002, 2005  Chris Nystrom
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *    For more license nformation see:
 *
 *      http://www.gnu.org/copyleft/gpl.html
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
#include <unistd.h>
#include "str_lib/str_lib.h"
#include "file_io_lib/file_io_lib.h"
#include "scp_lib/scp_lib.h"
#include "file_xfr.h"

#define NAME_SIZE (256)
#define LOGIN_SIZE (16)

PRIVATE int copy_file(const char *filename, const char *filepath);

PRIVATE char server[NAME_SIZE];
PRIVATE char app_name[NAME_SIZE];
PRIVATE char app_ver[NAME_SIZE];
PRIVATE path cwd;

PRIVATE char login_name[LOGIN_SIZE];
PRIVATE char password[LOGIN_SIZE];

PRIVATE path niodir;		// NIO cache dir
PRIVATE path dirname;
PRIVATE path filepath;

PUBLIC void
init_file_loader(const char *s, const char *an, const char *av,
		 const char *dir)
{
	strlcpy(server, s, NAME_SIZE);
	strlcpy(app_name, an, NAME_SIZE);
	strlcpy(app_ver, av, NAME_SIZE);
	pathcpy(cwd, (const char_ptr) dir);

//      printf("init_file_loader(): cwd = %s\n", cwd);

	get_home_dir(niodir);
	pathcat(niodir, "/.d0/cache/");
	pathcpy(dirname, niodir);
	check_dir(dirname);

	pathcat(dirname, server);
	pathcat(dirname, "/");
	check_dir(dirname);

	pathcat(dirname, app_name);
	pathcat(dirname, "/");
	check_dir(dirname);

	pathcat(dirname, app_ver);
	pathcat(dirname, "/");
	check_dir(dirname);

//      printf("init_file_loader(): dirname = %s\n", dirname);
}

PUBLIC void init_file_xfer(const char *login, const char *passwd)
{
	strlcpy(login_name, login, LOGIN_SIZE);
	strlcpy(password, passwd, LOGIN_SIZE);
}

PUBLIC char *load_file(const char *filename)
{
	int len;
	char *f;
	path file;

	len = (int) strlen(filename);

	if (len > PATH_SIZE) {
		fprintf(stderr, "load_file(): filename too big.\n");
		return (NULL);
	}

	pathcpy(file, (const char_ptr) filename);

	f = basename(file);

//      printf("load_file(): filename = %s\n", filename);

	pathcpy(filepath, dirname);
	pathcat(filepath, f);

//      printf("load_file(): filepath = %s\n", filepath);

	if (!file_exists(filepath)) {
		printf("File %s does not exist in cache.\n", f);
		printf("Copying from server...\n");
		copy_file(filename, filepath);
	}

	return (filepath);
}

PRIVATE int copy_file(const char *filename, const char *filepath)
{
#define COMMAND_SIZE  (512)

	char command[COMMAND_SIZE];

	strlcpy(command, login_name, COMMAND_SIZE);
	strlcat(command, "@", COMMAND_SIZE);
	strlcat(command, server, COMMAND_SIZE);
	strlcat(command, ":", COMMAND_SIZE);
//  strcat(command, cwd); 
	strlcat(command, "nio/", COMMAND_SIZE);
	strlcat(command, filename, COMMAND_SIZE);

	scp_file(command, filepath, password);

	if (!file_exists(filepath)) {
		fprintf(stderr, "copy_file(): Failed. Retrying...\n");
		scp_file(command, filepath, password);
		sleep(1);
	}

	if (!file_exists(filepath)) {
		fprintf(stderr,
			"copy_file(): Failed. Retrying again...\n");
		scp_file(command, filepath, password);
		sleep(2);
	}

	if (!file_exists(filepath)) {
		fprintf(stderr, "copy_file(): Failed.\n");
		return (-1);
	}

	return 0;
}
