# d0

d0 is Chris Nystrom's "application browser" (the name used in the
original TODO). It is an SDL-based client from the mid-2000s New IO /
Dreadnought work: a remote display program that talks over the network,
draws with SDL, and includes libraries for audio, events, images, messaging,
sockets, and a terminal.

The Changelog runs from 16 May 2005 through 14 Apr 2006, ending at
version 0.09, build 55.

## Origin

From Chris Nystrom's personal source archive (the hallie tree), posted
publicly at his request. The source in this repo is the 14 Apr 2006
`d0.0.09_src_055` snapshot. It is byte-identical to the `d0-0.09`
working directory in that archive (the tarball additionally contained
GNU FreeFont files and a `.wav`, which were not kept).

Years in the headers are typically 2002 and 2005.

## Pages

Chris's old programs page is at https://nystrom.family/chris/old/programs.html.
All of Chris's code: https://nystrom.family/chris/code/

## License

Most of Chris's files use the GNU LGPL 2.1 (see `header.lgpl`) or the
GNU GPL 2 (`header.gpl`). Read the header of each file. `movie_demo.c`
is marked public domain by Chris.

## Third-party files removed

These were in the original tree and were omitted because they were not
written by Chris:

- `src/str_lib/strlcat.c`, `src/str_lib/strlcpy.c` — Todd C. Miller, 1998
- `src/font_lib/fonts.c`, `text.c`, `font_lib.h` — major portions from
  Bob Pendleton's bounce.cpp, 2002, used by permission (Chris also put
  his own header on these files)
- `src/screen_lib/SDL_prim.c`, `SDL_prim.h` — Ryan McGuigan, 2003–2005
- `src/movie_lib/sdl_lib.c`, `sdl_lib.h` — Loki Entertainment Software
  plaympeg sample, 1999
- `src/audio_lib/extra/playlib.c`, `playlib.h` — lifted from Eric S.
  Raymond's speaker.c, 1990/1994
- `src/audio_lib/extra/speaker.c`, `speaker.h` — David Olofson, 2001
  (noted as modified by Chris in March 2003)
- GNU FreeFont files and their AUTHORS/README (fonts were not kept)

Compiled objects, the `d0` binary, and media (fonts, `newio.wav`,
`1.mpg`) were also stripped.

## Build

The project expects SDL, SDL_ttf, SDL_image, SDL_mixer, and `sdl-config`.
`make` in `src/` failed here on a missing `sdl-config` and then on
`SDL.h`. No source was modified. After the third-party removals, the
bundled Makefiles that still name the omitted files will also need
attention if you rebuild this historically.
