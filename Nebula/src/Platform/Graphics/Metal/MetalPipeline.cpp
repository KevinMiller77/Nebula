#include "MetalPipeline.h"

#include <Metal/Metal.hpp>

namespace Nebula {
    MetalPipeline::MetalPipeline(const PipelineSpecification& spec)
        : m_Specification(spec)
    {
        Invalidate();
    }

    MetalPipeline::~MetalPipeline()
    {
        m_VertexArray = nullptr;
    }

    void MetalPipeline::Invalidate()
    {
        m_VertexArray = VertexArray::Create();

    }

    void MetalPipeline::Bind()
    {
        if (m_VertexArray == nullptr) {
            LOG_ERR("[Metal][Pipeline] VertexArray not set!\n");
            abort();
        }

        m_Specification.m_Shader->Bind();
        m_VertexArray->Bind();
    }
}