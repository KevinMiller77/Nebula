#pragma once

#include <Math/math.h>
#include <Utils/Logging.h>
#include "VertexArray.h"

namespace Nebula
{
    class RendererAPI
    {
    public: 
        enum class API
        {
            None = 0, OpenGL = 1, Metal = 2, DirectX12 = 3
        };

        virtual ~RendererAPI() = default;

        virtual void Init() = 0;
        virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
        virtual void SetClearColor(const Vec4f& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const VertexArray* vertexArray, unsigned int indexCount = 0) = 0;

        static API GetAPI() { return NebAPI; }
        static RendererAPI* Create();
    private:
        static API NebAPI;

    };
}