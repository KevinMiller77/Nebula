#include <Graphics/RendererAPI.h>

namespace Nebula
{
    class GLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

		virtual void SetClearColor(const Vec4f& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const VertexArray* vertexArray, unsigned int indexCount = 0) override;
	};
}