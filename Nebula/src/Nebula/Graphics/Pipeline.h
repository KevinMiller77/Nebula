#pragma once

#include <Core/Ref.h>

#include "Shader.h"
#include "Buffer.h"
#include "RenderPass.h"

namespace Nebula{
    struct PipelineSpecification
    {
        Ref<Shader>     m_Shader;
        BufferLayout    m_Layout;
        Ref<RenderPass> m_RenderPass;
    };

    class Pipeline
    {
    public:
        virtual ~Pipeline() = default;

        virtual PipelineSpecification& GetSpec() = 0;
        virtual void SetSpec(const PipelineSpecification& spec) = 0;

        virtual void Invalidate() = 0;

        virtual void Bind() = 0;

        static Ref<Pipeline> Create(const PipelineSpecification& spec);
    };
}