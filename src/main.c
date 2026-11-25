/******************************************************************************
 *
 *    main.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SDL.h"
#include "font_lib/font_lib.h"
#include "sdl_lib/sdl_lib.h"
#include "audio_lib/audio_lib.h"
#include "screen_lib/screen_lib.h"
#include "color_lib/color_lib.h"
#include "rand.h"
#include "url.h"
#include "text_box.h"
#include "rpc.h"
#include "file_xfr.h"
#include "install_dir.h"

#define MAX_URL_LEN     (64)

#define HTTP  "http"
#define NIO   "nio"
#define DEFAULT_URL  "nio://appstest.newio.org:32662/list.nio"
//#define DEFAULT_URL  "nio://apps.newio.org:32662/list.nio"
//#define DEFAULT_URL  "nio://localhost:32662/list.nio"

extern void randomize(void);
extern void play_intro_sound(void);
extern void check_home_dir(void);
extern void get_url(char *s);
extern void get_login_info(char *username, char *password);
extern void call_http(const char *s);
extern void call_nio(url myurl);

void init_dnought(void);
void quit_dnought(void);

int main(void)
{
	char sURL[MAX_URL_LEN + 1] = DEFAULT_URL;
	url myurl;

	init_dnought();

	for (;;) {
		get_url(sURL);

		parse_url(sURL, &myurl);

		if (!strncmp(myurl.service, HTTP, 4)) {
			call_http(sURL);
		} else if (!strncmp(myurl.service, NIO, 4)) {
			char username[MAX_LINE_LEN] = "";
			char password[MAX_LINE_LEN] = "";

			get_login_info(username, password);

			if ((strlen(username) != 0)
			    && (strlen(password) != 0)) {
				init_file_xfer(username, password);
				call_nio(myurl);
			} else {
				printf("Bad login or password.\n");
			}
		}
	}

	quit_dnought();
}

void init_dnought(void)
{
	randomize();
	find_install_dir();
	check_home_dir();
	sdl_init();
	init_audio();
	play_intro_sound();
}

void quit_dnought(void)
{
	SDL_Quit();
	exit(0);
}
