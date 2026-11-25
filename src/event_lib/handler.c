/******************************************************************************
 *
 *    handler.c
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

#include <stdlib.h>
#include "../nio_events.h"
#include "user_event.h"
#include "event_lib.h"

extern int init_screen(int sw, int sh, int options);
extern void handle_key_event(SDL_KeyboardEvent * key);

PUBLIC void handle_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		int event_num;

		switch (event.type) {
		case SDL_VIDEORESIZE:
			init_screen(event.resize.w, event.resize.h,
				    SDL_ANYFORMAT | SDL_RESIZABLE);
			break;

		case SDL_MOUSEMOTION:
			mouse_move_handler(event.motion.x, event.motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			mouse_button_handler(event.type,
					     event.button.button);
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			handle_key_event(&event.key);
			break;

		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;

		case SDL_USEREVENT:
			event_num = event.user.code;

			if (event_num == NIO_EVENT_RESIZE) {
				event_resize();
			} else if (event_num == NIO_EVENT_MOUSE_CURSOR) {
				mouse_cursor();
			} else {
				printf("Unknown USEREVENT\n");
			}

			break;

		default:
			break;
		}
	}
}
