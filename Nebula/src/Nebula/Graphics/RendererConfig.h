#include "RendererAPI.h"

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
        
        static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
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

        static void DrawIndexed(const VertexArray* vertexArray, unsigned int indexCount = 0)
        {
            m_API->DrawIndexed(vertexArray, indexCount);
        }

    private:
        static RendererAPI* m_API;
    };
}