#pragma once

#include "Nebula/Core/PlatformInfo.h"

#ifdef NEB_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <iomanip>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <Debugging/Instrumentor.h>

#ifdef NEB_PLATFORM_WINDOWS
    #include <Windows.h>
#endif