
#include <unistd.h>
#include "../screen_lib/screen_lib.h"
#include "types.h"
#include "user_event.h"

static bool busy;

static int x;
static int y;

static int color;

static int cursor_status;

PUBLIC void setup_screen_resize_event(int x1, int y1, int c)
{
	x = x1;
	y = y1;

	color = c;

	busy = TRUE;
}

PUBLIC void event_resize(void)
{
	resize_screen(x, y);

	clear_screen(color, UPDATE);

	busy = FALSE;
}

PUBLIC void setup_mouse_cursor_event(int i)
{
	cursor_status = i;

	busy = TRUE;
}

PUBLIC void mouse_cursor(void)
{
	SDL_ShowCursor(cursor_status);

	busy = FALSE;
}

PUBLIC void event_busy(void)
{
	while (busy) {
		usleep(10000);
	}
}
