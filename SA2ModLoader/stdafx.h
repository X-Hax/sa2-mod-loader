// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <Windows.h>
#include <DbgHelp.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>

#include "ninja.h"
#include "SA2ModLoader.h"
#include "Events.h"
#include "FileReplacement.h"
#include "Trampoline.h"
#include "FunctionHook.h"