#pragma once

#include <Graphics/GraphicsContext.h>

struct GLFWwindow;

namespace Nebula
{
    class GLContext : public GraphicsContext
    {
    public:
        GLContext(GLFWwindow* window);

        virtual void Init() override;
        virtual void StartRender() override;
        virtual void SwapBuffers() override;
        virtual void ClearColor(Vec4f color) override; 

    private:
        GLFWwindow* window;
    };
}