// file name:	main.h
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#ifndef main_h
#define main_h

//includes
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx10.h>

#include "application.h"
#include "d3ddefs.h"

//globals
extern Application app;

//functions
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#endif