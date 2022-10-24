#pragma once

#include <Graphics/RenderPass.h>

namespace Nebula {

	class GLRenderPass : public RenderPass
	{
	public:
        GLRenderPass(const RenderPassSpecification& spec);
		virtual ~GLRenderPass() override;

		virtual RenderPassSpecification& GetSpecification() override { return m_Specification; }
		virtual const RenderPassSpecification& GetSpecification() const override { return m_Specification; }

    private:   
        RenderPassSpecification m_Specification;
	};

}