/******************************************************************************
 *
 *    screen_lib.h
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

#ifndef _SCREEN_LIB_H
#define _SCREEN_LIB_H

#ifndef _SDL_H
#include "SDL.h"
#endif

#ifndef __SDL_PRIM_H
#include "SDL_prim.h"
#endif

#ifndef PRIVATE
#define PRIVATE static
#endif

#ifndef PUBLIC
#define PUBLIC
#endif

#ifndef _BOOL_H
#define _BOOL_H

typedef enum {
	FALSE = 0,
	TRUE = 1
} bool;

#endif				// _BOOL_H

#define NO_UPDATE (0)
#define UPDATE (1)

#define NO_ALPHA (255)
#define MAX_ALPHA (0)

#define NO_FILL (0)
#define FILL (1)

PUBLIC int init_screen(int sw, int sh, int options);
PUBLIC int resize_screen(int w, int h);
PUBLIC SDL_PixelFormat *get_pixel_format(void);

PUBLIC void blit_screen(SDL_Surface * s, SDL_Rect * r);
PUBLIC void fill_rect(SDL_Rect * r, Uint32 color);
PUBLIC void clear_screen(Uint32 color, bool bUpdate);
PUBLIC void update_screen(void);
PUBLIC void update_rectangle(int x, int y, int w, int h);
PUBLIC void reset_screen(void);
PUBLIC int get_screen_width(void);
PUBLIC int get_screen_height(void);

//PUBLIC int clear_rectangle(int x, int y, int w, int h, Uint32 color);
PUBLIC int draw_rectangle(int x, int y, int w, int h, Uint32 color,
			  int alpha, int fill, bool update);
PUBLIC void draw_line(int x1, int y1, int x2, int y2, Uint32 color,
		      int alpha, int fill, bool bUpdate);
PUBLIC void draw_arc(int x, int y, int r, Uint32 color, int alpha,
		     int fill, bool bUpdate);
PUBLIC void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3,
			  Uint32 color, int alpha, int fill, bool bUpdate);
PUBLIC int draw_image(int i_num, int x, int y, bool bUpdate);
PUBLIC void draw_point(int x, int y, Uint32 color, bool bUpdate);

PUBLIC Uint32 getpixel(int x, int y);
PUBLIC void putpixel(int x, int y, Uint32 pixel);

#endif				// _SCREEN_LIB_H
