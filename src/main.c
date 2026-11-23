#include <stdio.h>
#include <X11/Xlib.h>

int main(void)
{
	// 1.Connect to display
    Display *display = XOpenDisplay(NULL);

    if (display == NULL)
    {
        printf("Could not connect to display\n");
        return 1;
    }

    //printf("Connected to display!\n");

	// 2. Get the default screen
	int screen = DefaultScreen(display);

	// 3. Create window
	Window window = XCreateSimpleWindow(
		display, 
		RootWindow(display, screen), 
		100, 100, 
		800, 600,
		1, 
		BlackPixel(display, screen),
		WhitePixel(display, screen)
	);

	// 4. Listen for window events
	XSelectInput(display, window, ExposureMask | KeyPressMask);

	// 5. Tell X11 about window closing
	Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &wmDelete, 1);

	// 6. Show window
	XMapWindow(display, window);

	// 7. Event loop
	int running = 1; /* true i think of it */

	while (running)
	{
		XEvent event; /* catch events? */

		XNextEvent(display, &event);

		if (event.type == Expose)
		{
			printf("Window exposed!\n");
		}

		if (event.type == KeyPress)
		{
			printf("Key pressed!\n");
		}

		if (event.type == ClientMessage)
		{
			if ((Atom)event.xclient.data.l[0] == wmDelete) /* i think it is if the user presses the 'X' button to close the window */
			{
				running = 0; /* running = false; */
			}
		}
	}

	// 8. Cleanup
	XDestroyWindow(display, window);
	XCloseDisplay(display);

    return 0;
}
