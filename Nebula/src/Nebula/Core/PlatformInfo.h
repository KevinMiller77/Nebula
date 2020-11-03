#pragma once

#include <memory>
#include <functional>
// Platform detection using predefined macros

#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define NEB_PLATFORM_WINDOWS
		#define NEB_PLATFORM_WIN64
	#else
		/* Windows x86 */
		
		#include <windows.h>
		#define NEB_PLATFORM_WINDOWS
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define NEB_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define NEB_PLATFORM_MACOS
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define NEB_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define NEB_PLATFORM_LINUX
#elif defined(__EMSCRIPTEN__)
	#define NEB_PLATFORM_EM
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

#define NEB_GAPI_OPENGL

#define BIT(x) (1 << x)

#define NEB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#define PI 3.14159265359f
#define PI2R_FACTOR PI / 180