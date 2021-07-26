#pragma once

#include <Core/Ref.h>
#include <Utils/Logging.h>
#include <Math/math.h>

namespace Nebula{
    class GraphicsContext
    {
    public:
        GraphicsContext() = default;

        virtual void Init() = 0;
        virtual bool SwapBuffers() = 0; 
        virtual void SetVSync(bool vsync) = 0;
        virtual void* GetNativeHandle() = 0;

        static Ref<GraphicsContext> Create(void* window);
    };

}