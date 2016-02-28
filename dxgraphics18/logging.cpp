#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <strsafe.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

//using namespace std;

#include "logging.h"

va_list va_alist; // add_log
std::ofstream ofile; // add_log
bool logOpened = false;

void __cdecl open_log (void)
{
	if(!logOpened)
	{
		logOpened = true;
		static char dir[] = "C:\\GitHub_repos\\dxgraphics18\\dxgraphics18\\Debug\\dxgraphics18Log.txt"; // For Vista because it can't write in Program Files
		ofile.open(dir, std::ios::app);
	}
}

void __cdecl close_log (void)
{
	if(ofile)
		ofile.close();
}

void __cdecl add_log (const char *fmt, ...)
{
	char logbuf[50000] = {0};

	if(!fmt) { return; }

	va_start (va_alist, fmt);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	if(ofile)
	{
		ofile << logbuf << std::endl;
	}
}
