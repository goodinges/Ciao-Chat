#ifdef _WIN32
#include "SidClientConnectionWin.cpp"
#elif defined (_SYMBIAN)
#include "SidClientConnectionSym.cpp"
#else
#include "SidClientConnectionUnx.cpp"
#endif

