#pragma once

#include <Graphics/RenderPass.h>

namespace Nebula {

	class MetalRenderPass : public RenderPass
	{
	public:
        MetalRenderPass(const RenderPassSpecification& spec);
		virtual ~MetalRenderPass() override;

		virtual RenderPassSpecification& GetSpecification() override { return m_Specification; }
		virtual const RenderPassSpecification& GetSpecification() const override { return m_Specification; }

    private:   
        RenderPassSpecification m_Specification;

        friend class MetalRendererAPI;
	};

}