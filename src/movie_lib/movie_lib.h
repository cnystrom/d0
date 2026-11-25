/*
   movie_lib.h
*/

#ifndef PRIVATE
#define PRIVATE static
#endif

#ifndef PUBLIC
#define PUBLIC
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

PUBLIC int movie_load(const char *filename, int use_audio);
PUBLIC void movie_print_info(const char *filename);
PUBLIC void moive_print_version(void);
PUBLIC void movie_enable_audio(int i);
PUBLIC void movie_enable_video(int i);
PUBLIC void movie_video_resize(int w, int h);
PUBLIC void moive_set_volume(int i);
PUBLIC void movie_volume_up(int delta);
PUBLIC void movie_volume_down(int delta);
PUBLIC void movie_volume_full(void);
PUBLIC void movie_volume_off(void);
PUBLIC void movie_toggle_play(void);
PUBLIC void movie_bilinear_filter_on(void);
PUBLIC void movie_bilinear_filter_off(void);
PUBLIC void movie_skip(int i);
PUBLIC void movie_set_display(void);
PUBLIC void movie_scale_XY(int x, int y);
PUBLIC void movie_loop(int i);
PUBLIC void movie_seek(Uint32 seek_val);
PUBLIC void movie_play(void);
PUBLIC void movie_delete(void);
PUBLIC int movie_playing(void);
PUBLIC int movie_has_video(void);
