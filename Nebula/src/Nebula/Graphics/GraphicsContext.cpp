
#include <Core/PlatformInfo.h>

#include <assert.h>
#include "GraphicsContext.h"

#include <Platform/Graphics/GL/GLContext.h>


namespace Nebula{
    Ref<GraphicsContext> GraphicsContext::Create(void* window)
    {
        #ifdef NEB_GAPI_OPENGL
            return CreateRef<GLContext>(window);
        #else
            LOG_ERR("No rendering API chosen!!!\n");
        #endif
    }
}