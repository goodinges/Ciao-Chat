
#ifndef _CLIENT_MAIN_H
#define _CLIENT_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "cskype.h"

#include "skype-object.h"
#include "command-invoker.h"
#include "helper-functions.h"

/** global variables */
extern CSkype* skype;

#ifdef SKYPEKIT_SURFACE_RENDERING
#if defined(GL_WINDOW) || defined(__WIN32)
class glWindow;
extern glWindow *preview_window;
extern glWindow *receive_window;
#else
#error "Defined SKYPEKIT_SURFACE_RENDERING but no XX_WINDOW defined"
#endif
#endif

#endif // _CLIENT_MAIN_H

