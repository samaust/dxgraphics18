// file name:	winproc.cpp
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#include "main.h"


/*
 *	Callback function that is invoked on every window event.
 *	Shut down application on pressing ESCAPE key.
 */
LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
		case WM_KEYDOWN:
		{
			if(wparam == VK_ESCAPE)
				DestroyWindow(handle);

			return 0;
		}
			break;

		case WM_DESTROY:
		{
			app.setRunning(false);
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(handle, message, wparam, lparam);
}