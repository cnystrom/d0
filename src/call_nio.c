/******************************************************************************
 *
 *    call_nio.c
 *
 *    Copyright (C) 2002, 2005  Chris Nystrom
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but without any warranty whatsoever.
 *
 *    For more license information see:
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

#include "SDL.h"
#include "sock_lib/sock_lib.h"
#include "msg_lib/msg_lib.h"
#include "sdl_lib/sdl_lib.h"
#include "screen_lib/screen_lib.h"
#include "event_lib/event_lib.h"
#include "font_lib/font_lib.h"
#include "audio_lib/audio_lib.h"
#include "image_lib/image_lib.h"
#include "url.h"
#include "done.h"

#define NIO_FILE_EXTENSION ".nio\n"
#define NIO_PORT  (32662)
#define ERR0  "error: no response from server.\n"
#define ERR1  "error: program not available.\n"
#define ERR2  "error: unknown return code.\n"

extern void nio_client_main_loop(void);

void call_nio(url myurl)
{
	char server[MAX_LINE_LEN];
	char program[MAX_LINE_LEN];
	char command[MAX_LINE_LEN * 3];
	int ret;
	int port_num = NIO_PORT;
	char str[BUFSIZ];
	char version[] = "0.09_bin_050\n";
	SOCKET s;

// This needs to go away and let app do it...
//      init_screen(640, 480, SDL_ANYFORMAT | SDL_RESIZABLE);

	set_window_caption(myurl.program);

	strcpy(server, myurl.server);
	strcpy(program, myurl.program);

	strcpy(command, program);
	strcat(command, NIO_FILE_EXTENSION);

	if (atoi(myurl.port) != 0) {
		port_num = atoi(myurl.port);
	}

	s = init_tcp_client(server, port_num);

	if (s < 0) {
		fprintf(stderr, "call_nio(): init_tcp_client() failed.\n");
		return;
	}
	// Get welcome message
	gets_socket(s, str, BUFSIZ);

	puts_socket(s, version);
	gets_socket(s, str, BUFSIZ);

	if (strlen(str) > 1) {
		system
		    ("cd ~; wget ftp://ftp.newio.org/pub/d0/update.sh; chmod +x update.sh");
		printf("Old browser version. Please run ~/update.sh.\n");
		exit(0);
	}

	puts_socket(s, command);
	gets_socket(s, str, BUFSIZ);

	ret = atoi(str);

	if (strlen(str) <= 0) {
		fprintf(stderr, ERR0);
	} else if (-1 == ret) {
		fprintf(stderr, ERR1);

	} else if (0 != ret) {
		fprintf(stderr, ERR2);
	} else {
		set_done(FALSE);
		msg_init_client(s, s);

		while (!done()) {
			handle_events();
		}

		msg_stop_client();
	}

	close_socket(s);
	free_all_fonts();
	delete_all_sounds();
	free_all_images();
}
