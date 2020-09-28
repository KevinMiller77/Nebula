#include "GLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Nebula
{
    GLContext::GLContext(GLFWwindow* window)
    {
        this->window = window;
    }

    void GLContext::Init()
    {
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_ERR("Could not init glad!!\n");
        }

        LOG_INF("OpenGL Info\n");
        LOG_INF("\t Vendor: %s\n", glGetString(GL_VENDOR));
        LOG_INF("\t Renderer: %s\n", glGetString(GL_RENDERER));
        LOG_INF("\t Version: %s\n", glGetString(GL_VERSION));        
    }

    void GLContext::StartRender()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLContext::SwapBuffers()
    {
        glfwSwapBuffers(window);
    }

    void GLContext::ClearColor(Vec4f color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }
}