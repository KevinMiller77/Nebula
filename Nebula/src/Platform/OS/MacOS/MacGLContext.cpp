#include <Platform/Graphics/GL/GLContext.h>

#include <glad/glad.h>
#include <Core/PlatformInfo.h>

namespace Nebula {
#ifdef NEB_PLATFORM_MACOS

    bool GLContext::InitContext_Int(void* window) {
        return true;
    }

    bool GLContext::SwapBuffers_Int() {
        return true;
    }

    void GLContext::SetVSync_Int(bool vsync) {}
#endif
}
