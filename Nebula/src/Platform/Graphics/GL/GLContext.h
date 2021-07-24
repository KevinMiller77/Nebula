#pragma once

#include <Core/Window.h>
#include <Graphics/GraphicsContext.h>

struct GLFWwindow;

namespace Nebula{
    class GLContext : public GraphicsContext
    {
    public:
        GLContext(void* window);

        virtual void Init() override;

        virtual bool SwapBuffers() override;
        virtual void SetVSync(bool vsync) override;

    private:

        // These functions are defined in the platform specific file [Platform]GLContext.cpp
        bool InitContext_Int(void* window);

        bool SwapBuffers_Int();
        void SetVSync_Int(bool vsync);
        /////////////////////////////////////////////////////////////////////////////////////

        void* s_DeviceContext = nullptr;
        void* s_RenderingContext = nullptr;

        void* s_Window = nullptr;
    };
}