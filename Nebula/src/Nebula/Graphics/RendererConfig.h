#pragma once

#include "RendererAPI.h"
#include <Core/NebulaCommon.h>
#include <Core/Ref.h>

namespace Nebula{
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

        static void Clear(Vec4f color)
        {
            m_API->Clear(color);
        }

        static void DrawIndexed(const int numVertices, PrimativeType type, bool depthTest)
        {
            m_API->DrawIndexed(numVertices, type, depthTest);
        }

        static void DrawElementsBaseVertex(uint32_t mode, uint32_t size, uint32_t type, void* indices, uint32_t base)
        {
            m_API->DrawElementsBaseVertex(mode, size, type, indices, base);
        }
        static void SetAlphaBlend(bool enabled)
        {
            m_API->SetAlphaBlend(enabled);
        }

        static void SetDepthTest(bool enabled)
        {
            m_API->SetDepthTest(enabled);
        }

        static void SetMSAA(bool enabled)
        {
            m_API->SetMSAA(enabled);
        }



    private:
        static Ref<RendererAPI> m_API;
    };
}