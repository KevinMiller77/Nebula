#pragma once

#include <Math/math.h>
#include <Utils/Logging.h>
#include "VertexArray.h"
#include <Core/NebulaCommon.h>
#include <Core/Ref.h>

namespace Nebula{
    enum PrimativeType
    {
        NONE = 0, TRIANGLES, LINES
    };
    
    class RendererAPI
    {
    public: 
        enum class API
        {
            None = 0, OpenGL = 1, Metal = 2, DirectX12 = 3
        };

        virtual ~RendererAPI() = default;

        virtual void Init() = 0;
        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
        virtual void SetClearColor(const Vec4f& color) = 0;
        virtual void Clear() = 0;
        virtual void Clear(Vec4f color) = 0; 

        virtual void SetAlphaBlend(bool enabled) = 0;
        virtual void SetDepthTest(bool enabled) = 0;
        virtual void SetMSAA(bool enabled) = 0;

        virtual void DrawIndexed(const int numVertices, PrimativeType type, bool depthTest) = 0;
        virtual void DrawElementsBaseVertex(uint32_t mode, uint32_t size, uint32_t type, void* indices, uint32_t base) = 0;

        static API GetAPI() { return NebAPI; }
        static Ref<RendererAPI> Create();
    private:
        static API NebAPI;

    };
}