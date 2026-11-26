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

This snapshot is d0 0.09 (14 Apr 2006), the historical predecessor of
the later d0 client in NewIO 0.12. This tree is kept as history and is
not being ported separately. For the later client, see
https://github.com/cnystrom/newio

## Pages

Chris's old programs page is at https://nystrom.family/chris/old/programs.html.
All of Chris's code: https://nystrom.family/chris/code/

## License

Most of Chris's files use the GNU LGPL 2.1 (see `header.lgpl`) or the
GNU GPL 2 (`header.gpl`). Read the header of each file. `movie_demo.c`
is marked public domain by Chris.

The GNU LGPL 2.1 (or later) text is in LICENSE; the GNU GPL 2 (or later) text is in COPYING. A few files are public domain, as marked in their headers.

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

The project expects SDL 1.2, SDL_ttf, SDL_image, and SDL_mixer.
`sdl-config` is fine on a current Linux system; that is no longer the
blocker. `make` in `src/` still does not build the application. The
real blockers are:

- missing `SDL_prim.h` (removed with Ryan McGuigan's primitives; see above)
- missing `font_lib` sources (the Bob Pendleton-derived files were removed)
- `movie_lib` includes `smpeg/smpeg.h`, which is not in this tree
- `basename` in `file_io_lib.h` clashes with glibc when `_GNU_SOURCE` is
  set (SDL flags can do that)

Some helper libraries under `src/` (audio, color, event, image, and sdl)
can still be built. The bundled Makefiles were updated so they no longer
name the omitted `strlcpy`/`strlcat` and `SDL_prim` sources, and a new
original `strlcpy`/`strlcat` (2026) was added under `src/str_lib/`. This
tree is not being ported separately; use NewIO's later d0 for the current
client.
