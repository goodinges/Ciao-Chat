#ifdef _WIN32
#include "SidServerConnectionWin.cpp"
#elif defined (_SYMBIAN)
#include "SidServerConnectionSym.cpp"
#else
#include "SidServerConnectionUnx.cpp"
#endif

