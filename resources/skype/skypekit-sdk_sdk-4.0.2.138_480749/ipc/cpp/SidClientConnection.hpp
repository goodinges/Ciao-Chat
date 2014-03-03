#ifndef Sid_SidClientConnection_INCLUDED_HPP
#define Sid_SidClientConnection_INCLUDED_HPP

#ifdef _WIN32
#include "SidClientConnectionWin.hpp"
#elif defined (_SYMBIAN)
#include "SidClientConnectionSym.hpp"
#else
#include "SidClientConnectionUnx.hpp"
#endif

#endif

