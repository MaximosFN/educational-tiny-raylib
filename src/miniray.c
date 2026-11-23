#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#include <time.h>
#include <stdint.h>

#include "miniray.h"

// Internal state
static Display *display; /* retains its value between function calls */ /* when a function gets called 
																		multiple times the value of the static var
																		remains the same */
static Window window;
static Atom wmDelete;

static int shouldClose = 0; /* false */


static double previousTime = 0.0;
static float frameTime = 0.0f;
static int fps = 0;

static GC graphicsContext;


void InitWindow(int width, int height, const char *title)
{
	display = XOpenDisplay(NULL);

	if (display == NULL)
	{
		printf("MiniRay: Could not connect to display\n");
		shouldClose = 1; /* true (closes the window) */
		return;
	}

	int screen = DefaultScreen(display); /* default screen number */ /* i only have one monitor (0) */

	window = XCreateSimpleWindow(
		display, 
		RootWindow(display, screen),  /* make the new window child of the desktop */
		100, 100,  /* initial cords */
		width, height, 
		1, /* border width */
		BlackPixel(display, screen), /* border color (black)*/
		WhitePixel(display, screen)  /* window color (white) */
	);

	graphicsContext = XCreateGC(display, window, 0, NULL);

	XStoreName(display, window, title); /* setWindowTitle */

	XSelectInput(display, window, ExposureMask | KeyPressMask); /* which events the application listens for */

	XSetWMProtocols(display, window, &wmDelete, 1); /* close window when user presses 'X' */

	XMapWindow(display, window);

	shouldClose = 0; /* false */
}

int WindowShouldClose(void)
{
	if (display == NULL)
		return 1;

	while (XPending(display))
	{
		XEvent event;

		XNextEvent(display, &event);

		if (event.type == ClientMessage)
		{
			if ((Atom)event.xclient.data.l[0] == wmDelete)
			{
				shouldClose = 1; /* true (the user pressed "X") */
			}
		}
	}

	return shouldClose; /* if user didnt press X it remains 0 else it becomes true and the window closes */
}

void CloseWindow(void)
{
	if (display == NULL)
		return;

	XFreeGC(display, graphicsContext);

	XDestroyWindow(display, window); /* tells Xlib to delete the window and remove it from the screen */
	XCloseDisplay(display);

	display = NULL;
}

static double GetTime(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts); /* clock designed for measuring elapsed time */

	return (double)ts.tv_sec + 
			(double)ts.tv_nsec / 1000000000.0;
}

float GetFrameTime(void)
{
	return frameTime;
}

int GetFPS(void)
{
	return fps;
}

void BeginDrawing(void)
{
	// Start drawing a new frame
}

void EndDrawing(void)
{
	double currentTime = GetTime();

	frameTime = (float)(currentTime - previousTime);

	previousTime = currentTime;

	if (frameTime > 0.0f)
	{
		fps = (int)(1.0f / frameTime);
	}

	XFlush(display); /* sends buffered requests to the display server */
}

/* helper to convert our colors into X11 colors */
static unsigned long ConvertColor(Color color)
{
	return ((unsigned long)(color.r * 257) << 16) |
			((unsigned long)(color.g * 257) << 8) |
			((unsigned long)(color.b * 257));
}

void ClearBackground(Color color)
{
	XSetForeground(display, graphicsContext, ConvertColor(color));

	XFillRectangle(
		display, 
		window, 
		graphicsContext, 
		0, 0, 
		800, 600
	);
}

void DrawPixel(int x, int y, Color color)
{
	XSetForeground(display, graphicsContext, ConvertColor(color));

	XDrawPoint(display, window, graphicsContext, x, y); 
}

void DrawRectangle(int x, int y, int width, int height, Color color)
{
	XSetForeground(display, graphicsContext, ConvertColor(color));

	XFillRectangle(
		display, 
		window, 
		graphicsContext, 
		x, y, 
		width, height
	);
}

void DrawCircle(int centerX, int centerY, int radius, Color color)
{
	XSetForeground(display, graphicsContext, ConvertColor(color));

	XFillArc(
		display, 
		window, 
		graphicsContext, 
		centerX - radius, 
		centerY - radius, 
		radius * 2, 
		radius * 2, 
		0, 
		360 * 64
	);
}
