#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include <smpeg/smpeg.h>

int smpeg_load(int x, int y, const char *filename);
void smpeg_play(void);
void smpeg_delete(void);
int smpeg_playing(void);
