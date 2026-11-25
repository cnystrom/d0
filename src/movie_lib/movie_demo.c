/******************************************************************************
 *
 *    movie_demo.c
 *
 *    Copyright (c) 2005  Chris Nystrom
 *
 *    This program is public domain.
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

#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include <smpeg/smpeg.h>
#include "sdl_lib.h"
#include "movie_lib.h"

static int done = 0;

int main(int argc, char *argv[])
{
	sdl_init();
	sdl_setup_video(640, 480);

	movie_load("1.mpg", TRUE);
	movie_scale_XY(320, 240);
	movie_set_display();
	movie_play();

	while (!done && (movie_playing())) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = 1;
				break;
			case SDL_QUIT:
				done = 1;
				break;
			default:
				break;
			}
		}

		SDL_Delay(200);
	}

	movie_delete();

	SDL_Quit();

	return 0;
}
