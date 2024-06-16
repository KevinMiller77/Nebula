#include <Graphics/Pipeline.h>
#include <Graphics/VertexArray.h>

namespace Nebula {
    class MetalPipeline : public Pipeline{
    public:
        MetalPipeline(const PipelineSpecification& spec);
        virtual ~MetalPipeline() override;

        virtual PipelineSpecification& GetSpec() override { return m_Specification; }
        virtual void SetSpec(const PipelineSpecification& spec) override { m_Specification = spec; Invalidate(); }

        virtual void Invalidate() override;

        virtual void Bind() override;

    private:
		PipelineSpecification m_Specification;
		Ref<VertexArray> m_VertexArray;
    };
}