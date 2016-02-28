#ifndef _LOGGING_H
#define _LOGGING_H

//#define LOG_TO_FILE // comment to disable logging to file

void __cdecl open_log (void);
void __cdecl close_log (void);
void __cdecl add_log (const char *fmt, ...);

#ifdef LOG_TO_FILE
#define ADD_LOG(fmt, ...) { add_log(fmt, __VA_ARGS__); }
#else
#define ADD_LOG(fmt, ...) {}
#endif

#endif