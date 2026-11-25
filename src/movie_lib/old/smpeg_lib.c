/******************************************************************************
 *
 *    smpeg_lib.c
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
#include <stdio.h>
#include "SDL.h"
#include <smpeg/smpeg.h>

extern SDL_Surface *get_screen(void);

SMPEG *mpeg;			// smpeg file buffer
SMPEG_Info info;		// smpeg file information
SDL_Rect dst;			// destination coordinates on screen
SDL_Surface *surf_1;		// destination SDL surface

void updateMPEGFrame(SDL_Surface * buffer, Sint32 x, Sint32 y,
		     Uint32 w, Uint32 h)
{
	SDL_Surface *screen = get_screen();

	SDL_BlitSurface(surf_1, NULL, screen, &dst);
	SDL_Flip(screen);
}

int smpeg_load(int x, int y, const char *filename)
{
	SDL_Surface *sTemp;
	Uint32 rmask, gmask, bmask, amask;

	dst.x = x;
	dst.y = y;
	dst.w = 0;
	dst.h = 0;

	// Create surface,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff0000ff;
#endif

	sTemp =
	    SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 400, 32, rmask, gmask,
				 bmask, amask);

	if (sTemp == NULL) {
		fprintf(stderr, "%s:%u Failed to create RGB surface\n",
			__FILE__, __LINE__);
		return -1;
	}

	surf_1 = SDL_DisplayFormat(sTemp);

	mpeg = SMPEG_new(filename, &info, 1);

	if (!info.has_video) {
		fprintf(stderr, "%s:%u no video found\n", __FILE__,
			__LINE__);
		return -1;
	}

	return 0;
}

void smpeg_play(void)
{
	SMPEG_setdisplay(mpeg, surf_1, NULL, updateMPEGFrame);
	SMPEG_play(mpeg);
}

void smpeg_delete(void)
{
	SMPEG_delete(mpeg);
}

int smpeg_playing(void)
{
	if (SMPEG_status(mpeg) == SMPEG_PLAYING) {
		return 1;
	}

	return 0;
}
