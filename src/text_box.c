/******************************************************************************
 *
 *    text_box.c
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
#include <SDL.h>
#include "font_lib/font_lib.h"
#include "screen_lib/screen_lib.h"
#include "color_lib/color_lib.h"
#include "ascii_codes.h"
#include "text_box.h"

PRIVATE void text_box_insert_char(text_box_ptr tb, int c);
PRIVATE void text_box_loop(text_box_ptr tb);
PRIVATE void draw_text_box(text_box_ptr tb);
PRIVATE void text_box_show_cursor(text_box_ptr tb, int color);
PRIVATE char *get_mask_string(text_box_ptr tb);

PRIVATE char mask_string[TB_MAX_COL + 1];

PUBLIC int text_box(text_box_ptr tb)
{
	SDL_Rect r;
	glyphFont gf;

	if (tb->box_size > TB_MAX_COL) {
		fprintf(stderr, "draw_text_box(): size too big.\n");
		return (-1);
	}

	tb->index = strlen(tb->str);


	if (tb->index > tb->box_size) {
		fprintf(stderr, "draw_text_box(): ");
		fprintf(stderr, "default entry larger than text box.\n");
		return (-2);
	}

	gf = get_font(tb->font_num);
	text_size(&gf, "X", &r);
	tb->font_width = r.w;
	tb->font_height = gf.height;

	draw_text_box(tb);
	text_box_loop(tb);

	return (0);
}

PRIVATE void text_box_insert_char(text_box_ptr tb, int c)
{
	if (c == BS) {
		if (tb->index > 0) {
			tb->str[--tb->index] = '\0';
		}

		draw_text_box(tb);
	} else if (tb->index < tb->box_size) {
		tb->str[tb->index++] = c;
		tb->str[tb->index] = '\0';
	}

	draw_text_box(tb);
}

PRIVATE void text_box_loop(text_box_ptr tb)
{
	int key;
	SDL_Event ev;

	while (SDL_WaitEvent(&ev)) {
		switch (ev.type) {

		case SDL_KEYDOWN:
			key = ev.key.keysym.unicode & 0x7F;

			if (ESC == key) {
				exit(0);
			}

			if (CR == key) {
				return;
			}

			if (NUL != key) {
				text_box_insert_char(tb, key);
			}
			break;

		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;
		}
	}
}

PRIVATE void draw_text_box(text_box_ptr tb)
{
	Uint32 color;
	glyphFont gf;

	gf = get_font(tb->font_num);

	draw_rectangle(tb->x_home, tb->y_home,
		       (tb->font_width - 1) * (tb->box_size + 2),
		       tb->font_height, tb->fg_color, NO_ALPHA, FILL,
		       TRUE);

	if (tb->pw_flag) {
		draw_text(&gf, get_mask_string(tb),
			  tb->x_home + tb->font_width, tb->y_home, TRUE);
	} else {
		draw_text(&gf, tb->str,
			  tb->x_home + tb->font_width, tb->y_home, TRUE);
	}

	color = get_rand_color();
	text_box_show_cursor(tb, color);
}

PRIVATE void text_box_show_cursor(text_box_ptr tb, int color)
{
	draw_rectangle(tb->x_home +
		       ((tb->index + 1) * (tb->font_width - 1)) + 2,
		       tb->y_home, 2, 20, color, NO_ALPHA, FILL, TRUE);
}

PRIVATE char *get_mask_string(text_box_ptr tb)
{
	int i, len;

	len = strlen(tb->str);

	for (i = 0; i < len; i++) {
		mask_string[i] = '*';
	}

	mask_string[len] = '\0';

	return (mask_string);
}
