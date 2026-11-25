/******************************************************************************
 *
 *    do_data_in.c
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
#include "msg_lib/msg_lib.h"
#include "nio.h"
#include "rpc.h"
#include "done.h"

extern void print_func(int fn);

void do_data_in(void)
{
	int func_num;

	func_num = msg_get_num();

//      print_func(func_num);

	if (func_num == NIO_OUT_INIT)
		nio_app_init_1();
	else if (func_num == NIO_OUT_EXIT) {
		nio_exit_1();
	} else if (func_num == NIO_OUT_CHAR_TO_STDOUT)
		nio_char_to_stdout_1();
	else if (func_num == NIO_OUT_STRING_TO_STDOUT)
		nio_string_to_stdout_1();
	else if (func_num == NIO_OUT_INT_TO_STDOUT)
		nio_int_to_stdout_1();
	else if (func_num == NIO_OUT_PUT_CHAR)
		nio_put_char_1();
	else if (func_num == NIO_OUT_SYSTEM_LOCAL)
		nio_system_local_1();
	else if (func_num == NIO_OUT_LOAD_FONT)
		nio_load_font_1();
	else if (func_num == NIO_OUT_LOAD_SYS_FONT)
		nio_load_sys_font_1();
	else if (func_num == NIO_OUT_FREE_FONT)
		nio_free_font_1();
	else if (func_num == NIO_OUT_DRAW_TEXT)
		nio_draw_text_1();
	else if (func_num == NIO_OUT_SET_SCREEN)
		nio_set_screen_1();
	else if (func_num == NIO_OUT_CLEAR_SCREEN)
		nio_clear_screen_1();
	else if (func_num == NIO_OUT_DRAW_RECTANGLE_RGB)
		nio_draw_rectangle_rgb_1();
	else if (func_num == NIO_OUT_DRAW_POINT_RGB)
		nio_draw_point_rgb_1();
	else if (func_num == NIO_OUT_DRAW_LINE_RGB)
		nio_draw_line_rgb_1();
	else if (func_num == NIO_OUT_DRAW_ARC_RGB)
		nio_draw_arc_rgb_1();
	else if (func_num == NIO_OUT_SOUND)
		nio_sound_1();
	else if (func_num == NIO_OUT_BEEP)
		nio_beep_1();
	else if (func_num == NIO_OUT_LOAD_MUSIC)
		nio_load_music_1();
	else if (func_num == NIO_OUT_PLAY_MUSIC)
		nio_play_music_1();
	else if (func_num == NIO_OUT_STOP_MUSIC)
		nio_stop_music_1();
	else if (func_num == NIO_OUT_LOAD_SOUND)
		nio_load_sound_1();
	else if (func_num == NIO_OUT_PLAY_SOUND)
		nio_play_sound_1();
	else if (func_num == NIO_OUT_FREE_SOUND)
		nio_free_sound_1();
//      else if (func_num == NIO_OUT_LOAD_MOVIE)
//              nio_load_movie_1();
//      else if (func_num == NIO_OUT_PLAY_MOVIE)
//              nio_play_movie_1();
	else if (func_num == NIO_OUT_LOAD_IMAGE)
		nio_load_image_1();
	else if (func_num == NIO_OUT_DRAW_IMAGE)
		nio_draw_image_1();
	else if (func_num == NIO_OUT_FREE_IMAGE)
		nio_free_image_1();
	else if (func_num == NIO_OUT_TERM_INIT)
		nio_term_init_1();
	else if (func_num == NIO_OUT_PRINT)
		nio_print_1();
	else if (func_num == NIO_OUT_PRINT_AT)
		nio_print_at_1();
	else if (func_num == NIO_OUT_PAINT)
		nio_paint_1();
	else if (func_num == NIO_OUT_CURSOR)
		nio_cursor_1();
	else {
		printf
		    ("do_data_in(): bad function number = %d\n", func_num);
//              exit(-1);
	}
}
