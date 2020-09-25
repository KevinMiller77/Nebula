#pragma once

#include <Utils/Logging.h>
#include <Math/math.h>

namespace Nebula
{
    class GraphicsContext
    {
    public:
        GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void StartRender() = 0;
        virtual void ClearColor(Vec4f color) = 0; 

        static GraphicsContext* Create(void* window);
    };

}