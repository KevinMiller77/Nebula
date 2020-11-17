#pragma once

#include <Core/Ref.h>

#include "Shader.h"
#include "Buffer.h"

namespace Nebula
{
    struct PipelineSpec
    {
        Ref<Shader> plShader;
        BufferLayout plLayout;
    };

    class Pipeline
    {
    public:
        virtual ~Pipeline() = default;

        virtual PipelineSpec& GetSpec() = 0;
        virtual const PipelineSpec SetSpec() = 0;

        virtual void Invalidate() = 0;

        virtual void Bind() = 0;

        static Ref<Pipeline> Create(const PipelineSpec& spec);
    };
}