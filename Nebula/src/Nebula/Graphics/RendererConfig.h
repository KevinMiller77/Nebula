#pragma once

#include "RendererAPI.h"
#include <Core/NebulaCommon.h>

namespace Nebula
{
    class RendererConfig
    {
    public:
        virtual ~RendererConfig() = default;

        static void Init()
        {
            m_API->Init();
        }
        
        static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
        {
            m_API->SetViewport(x, y, width, height);
        }
        
        static void SetClearColor(const Vec4f& color)
        {
            m_API->SetClearColor(color);
        }

        static void Clear()
        {
            m_API->Clear();
        }

        static void DrawIndexed(const VertexArray* vertexArray, uint32 indexCount = 0)
        {
            m_API->DrawIndexed(vertexArray, indexCount);
        }

    private:
        static RendererAPI* m_API;
    };
}