
#include <Core/PlatformInfo.h>

#include <assert.h>
#include "GraphicsContext.h"

#include <Platform/Graphics/GL/GLContext.h>

#include <GLFW/glfw3.h>


namespace Nebula
{
    GraphicsContext* GraphicsContext::Create(void* window)
    {
        #ifdef NEB_GAPI_OPENGL
            return new GLContext((GLFWwindow*)window)
            
            
            
            
            
            
            ;
        #else
            LOG_ERR("No rendering API chosen!!!\n");
        #endif
    }
}