#include "GLContext.h"

#include <Nebula_pch.h>

#include <Core/PlatformInfo.h>
#include <Core/Window.h>

#include <glad/glad.h>

namespace Nebula{


    GLContext::GLContext(void* window)
    {
        s_Window = window;
    }

    void GLContext::Init()
    {
        NEB_PROFILE_FUNCTION();


        if(!InitContext_Int(s_Window)) {
            LOG_ERR("Could not init OpenGL context!");
            assert(false);
        }

        LOG_INF("OpenGL Init Success!\n");
        LOG_INF("\tGL Info\n");
        LOG_INF("\t Vendor: %s\n", glGetString(GL_VENDOR));
        LOG_INF("\t Renderer: %s\n", glGetString(GL_RENDERER));
        LOG_INF("\t Version: %s\n", glGetString(GL_VERSION));        
    }

    bool GLContext::SwapBuffers()
    {
        return SwapBuffers_Int();
    }

    void GLContext::SetVSync(bool vsync) {
        SetVSync_Int(vsync);
    }
}