#pragma once

#include <Graphics/Pipeline.h>

namespace Nebula {
    class GLPipeline : public Pipeline{
    public:
        GLPipeline(const PipelineSpecification& spec);
        virtual ~GLPipeline() override;

        virtual PipelineSpecification& GetSpec() override { return m_Specification; }
        virtual void SetSpec(const PipelineSpecification& spec) override { m_Specification = spec; Invalidate(); }

        virtual void Invalidate() override;

        virtual void Bind() override;

    private:
		PipelineSpecification m_Specification;
		uint32_t m_VertexArrayRendererID = 0;
    };
}