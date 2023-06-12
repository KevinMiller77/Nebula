#pragma once

#include <Core/Window.h>
#include <Graphics/GraphicsContext.h>
#include <Metal/Metal.hpp>

namespace Nebula {
    class MetalContext : public GraphicsContext {
    public:
        MetalContext(void* window);

        virtual void Init() override;

        virtual bool SwapBuffers() override;
        virtual void SetVSync(bool vsync) override;

    private:

        // These functions are defined in the platform specific file [Platform]MetalContext.cpp
        bool InitContext_Int(void* window);

        bool SwapBuffers_Int();
        void SetVSync_Int(bool vsync);
        /////////////////////////////////////////////////////////////////////////////////////
        
        void* m_Window = nullptr;

        MTL::Device* m_Device = nullptr;
        MTL::CommandQueue* m_CommandQueue = nullptr;

        MTL::RenderPipelineState* m_RenderPipelineState = nullptr;
        MTL::DepthStencilState* m_DepthStencilState = nullptr;
        
        MTL::ClearColor m_ClearColor;
        MTL::Viewport m_Viewport;

        MTL::CommandBuffer* m_CommandBuffer = nullptr;
        MTL::RenderCommandEncoder* m_RenderCommandEncoder = nullptr;

        MTL::Texture* m_Texture = nullptr;
        MTL::Buffer* m_VertexBuffer = nullptr;
        MTL::Buffer* m_IndexBuffer = nullptr;

        MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;
        MTL::Texture* m_DepthTexture = nullptr;

        MTL::CommandBuffer* m_FrameCommandBuffer = nullptr;
        MTL::RenderCommandEncoder* m_FrameRenderCommandEncoder = nullptr;
        
        bool m_AlphaBlendEnabled = false;
        bool m_DepthTestEnabled = false;
        bool m_MSAAEnabled = false;

        friend class MetalRendererAPI;
        friend class MacWindow;
    };
}
