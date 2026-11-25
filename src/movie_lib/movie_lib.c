/*
   movie_lib.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef unix
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#endif

#include "SDL.h"
#include "smpeg/smpeg.h"
#include "../screen_lib/screen_lib.h"
#include "movie_lib.h"

typedef struct {
	SMPEG *mpeg;
	SMPEG_Info info;
	SDL_Surface *tmp_surf;
	SDL_Rect rect;
	int volume;
} movie;

PRIVATE movie m;

PUBLIC void movie_update(SDL_Surface * buffer, Sint32 x, Sint32 y,
			 Uint32 w, Uint32 h)
{
//      SDL_Surface *screen = sdl_get_screen();

//      SDL_BlitSurface(m.tmp_surf, NULL, screen, &m.rect);

	blit_screen(m.tmp_surf, &m.rect);

	update_rectangle(m.rect.x, m.rect.y, m.rect.w, m.rect.h);

//      SDL_UpdateRect(screen, 0, 0, 0, 0);
}

PUBLIC int movie_load(const char *filename, int use_audio)
{
//      SDL_Surface *sTemp;
	Uint32 rmask, gmask, bmask, amask;

	m.mpeg = SMPEG_new(filename, &m.info, use_audio);

	if (SMPEG_error(m.mpeg)) {
		fprintf(stderr, "%s: %s\n", filename, SMPEG_error(m.mpeg));
		SMPEG_delete(m.mpeg);
		return (-1);
	}
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

	m.tmp_surf =
	    SDL_CreateRGBSurface(SDL_SWSURFACE, m.info.width,
				 m.info.height, 32, rmask, gmask, bmask,
				 amask);
	if (m.tmp_surf == NULL) {
		fprintf(stderr, "%s:%u Failed to create RGB surface\n",
			__FILE__, __LINE__);
		return -1;
	}

	m.rect.x = 50;
	m.rect.y = 50;
	m.rect.w = 0;
	m.rect.h = 0;

	return 0;
}

PUBLIC void movie_print_info(const char *filename)
{
	if (m.info.has_audio && m.info.has_video) {
		printf("%s: MPEG system stream (audio/video)\n", filename);
	} else if (m.info.has_audio) {
		printf("%s: MPEG audio stream\n", filename);
	} else if (m.info.has_video) {
		printf("%s: MPEG video stream\n", filename);
	}

	if (m.info.has_video) {
		printf("\tVideo %dx%d resolution\n", m.info.width,
		       m.info.height);
	}

	if (m.info.has_audio) {
		printf("\tAudio %s\n", m.info.audio_string);
	}

	if (m.info.total_size) {
		printf("\tSize: %d\n", m.info.total_size);
	}

	if (m.info.total_time) {
		printf("\tTotal time: %f\n", m.info.total_time);
	}
}

PUBLIC void movie_print_smpeg_version(void)
{
	SMPEG_version smpegver;

	SMPEG_VERSION(&smpegver);

	printf("SMPEG version: %d.%d.%d\n",
	       smpegver.major, smpegver.minor, smpegver.patch);
}

PUBLIC void movie_enable_audio(int i)
{
	SMPEG_enableaudio(m.mpeg, i);
}

PUBLIC void movie_enable_video(int i)
{
	SMPEG_enablevideo(m.mpeg, i);
}

PUBLIC void movie_video_resize(int w, int h)
{
	SMPEG_pause(m.mpeg);

	if (resize_screen(w, h)) {
		movie_set_display();
	}

	SMPEG_scaleXY(m.mpeg, w, h);
	SMPEG_pause(m.mpeg);
}

PUBLIC void movie_set_volume(int i)
{
	if (i < 0) {
		i = 0;
	}

	if (i > 100) {
		i = 100;
	}

	m.volume = i;

	SMPEG_setvolume(m.mpeg, m.volume);
}

PUBLIC void movie_volume_up(int delta)
{
	if (m.volume < 100) {
		m.volume += delta;

		if (m.volume > 100)
			m.volume = 100;

		SMPEG_setvolume(m.mpeg, m.volume);
	}
}

PUBLIC void movie_volume_down(int delta)
{
	if (m.volume > 0) {
		m.volume -= delta;

		if (m.volume < 0)
			m.volume = 0;

		SMPEG_setvolume(m.mpeg, m.volume);
	}
}

PUBLIC void movie_volume_full(void)
{
	m.volume = 100;
	SMPEG_setvolume(m.mpeg, m.volume);
}

PUBLIC void movie_volume_off(void)
{
	m.volume = 0;
	SMPEG_setvolume(m.mpeg, m.volume);
}

PUBLIC void movie_toggle_play(void)
{
	if (SMPEG_status(m.mpeg) == SMPEG_PLAYING) {
		SMPEG_pause(m.mpeg);
	} else {
		SMPEG_play(m.mpeg);
	}
}

PUBLIC void movie_bilinear_filter_on(void)
{
	SMPEG_Filter *filter = SMPEGfilter_bilinear();

	filter = SMPEG_filter(m.mpeg, filter);

	filter->destroy(filter);
}

PUBLIC void movie_bilinear_filter_off(void)
{
	SMPEG_Filter *filter = SMPEGfilter_null();

	filter = SMPEG_filter(m.mpeg, filter);

	filter->destroy(filter);
}

PUBLIC void movie_skip(int i)
{
	SMPEG_skip(m.mpeg, i);
}

PUBLIC void movie_set_display(void)
{
	SMPEG_setdisplay(m.mpeg, m.tmp_surf, NULL, movie_update);
}

PUBLIC void movie_scale_XY(int x, int y)
{
	SMPEG_scaleXY(m.mpeg, x, y);
}

PUBLIC void movie_loop(int i)
{
	SMPEG_loop(m.mpeg, i);
}

PUBLIC void movie_seek(Uint32 seek_val)
{
	SMPEG_seek(m.mpeg, seek_val);
}

PUBLIC void movie_play(void)
{
	SMPEG_play(m.mpeg);
}

PUBLIC void movie_delete(void)
{
	SMPEG_delete(m.mpeg);
}

PUBLIC int movie_playing(void)
{
	if (SMPEG_status(m.mpeg) == SMPEG_PLAYING) {
		return 1;
	}

	return 0;
}

PUBLIC int movie_has_video(void)
{
	if (m.info.has_video) {
		return 1;
	}

	return 0;
}
