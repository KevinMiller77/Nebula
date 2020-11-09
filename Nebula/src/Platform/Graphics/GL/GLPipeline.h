#pragma once

#include <Graphics/Pipeline.h>

namespace Nebula
{
    class GLPipeline : public Pipeline
    {
    public:
        virtual ~Pipeline() = default;

        virtual PipelineSpec& GetSpec() = 0;
        virtual const PipelineSpec SetSpec() = 0;

        virtual void Invalidate() = 0;

        virtual void Bind() = 0;

    private:
    
    };
}