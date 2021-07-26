#include <Graphics/RendererAPI.h>
#include <Core/NebulaCommon.h>

namespace Nebula{
    class GLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;

		virtual void SetClearColor(const Vec4f& color) override;
		virtual void Clear() override;
        virtual void Clear(Vec4f color) override;

		virtual void SetAlphaBlend(bool enabled) override;
        virtual void SetDepthTest(bool enabled) override;
        virtual void SetMSAA(bool enabled) override;

		virtual void DrawIndexed(const int numIndices, PrimativeType type, bool depthTest) override;
        virtual void DrawElementsBaseVertex(PrimativeType mode, uint32_t size, PrimativeDataType type, void* indices, uint32_t base) override;
	};
}