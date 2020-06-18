// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "EventTypeInfo.h"
#include "ViewId.h"
#include "StringHash.h"
#include "ForwardDeclarations.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <memory>
#include <functional>
#include <cassert>
#include <chrono>
#include <ctime>

#include "logger/Logger.h"

#if defined(_WIN64) // using portable common solution for x64 configuration
#include <crtdbg.h>
#define DEBUG_BREAK_IF( cond ) if(cond) {_CrtDbgBreak();}
#else
#define DEBUG_BREAK_IF( cond ) if(cond) {_asm int 3}
#endif

#endif //PCH_H
