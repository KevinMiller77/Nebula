#include <Graphics/RendererAPI.h>
#include <Core/NebulaCommon.h>

namespace Nebula
{
    class GLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;

		virtual void SetClearColor(const Vec4f& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray> vertexArray, PrimativeType type, uint32 indexCount = 0) override;
	};
}