#pragma once

#include <Core/Window.h>
#include <Graphics/GraphicsContext.h>
#include <Platform/Graphics/Metal/MetalRendererAPI.h>

#include <Metal/Metal.hpp>

namespace MTK {
    class View;
}

namespace NS {
    class Window;

}

namespace MTL {
    class Device;
    class CommandQueue;
    class CommandBuffer;
    class RenderCommandEncoder;
    class RenderPipelineState;
    class RenderPassDescriptor;
    class ClearColor;
    class Viewport;
    class Buffer;
    class Texture;
}

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
        
        NS::Window* m_Window = nullptr;
        
        MTK::View* m_DefaultContentView = nullptr;
        MTK::View* m_BoundView = nullptr;
        MTK::View* m_NextView = nullptr;

        bool m_AlphaBlendEnabled = false;
        bool m_DepthTestEnabled = false;
        bool m_MSAAEnabled = false;

        // State only modified by the renderer
        MTL::Device* m_Device = nullptr;
        MTL::CommandQueue* m_CommandQueue = nullptr;
        MTL::CommandBuffer* m_CommandBuffer = nullptr;
        MTL::RenderCommandEncoder* m_RenderCommandEncoder = nullptr;

        /**
         * @brief The below variables are renderer state currently modified by 
            binding various components of the renderer.
         * 
         */

        // Shader
        MTL::RenderPipelineState* m_RenderPipelineState = nullptr;

        // Framebuffer & RenderPass
        MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;
        
        // Window holds this data
        MTL::ClearColor m_ClearColor;
        MTL::Viewport m_Viewport;

        // Vertex Array binding
        MTL::Buffer* m_VertexBuffer = nullptr;
        MTL::Buffer* m_IndexBuffer = nullptr;

        MTL::Texture* m_DepthTexture = nullptr;

        MTL::RenderPassColorAttachmentDescriptor* m_FrameRenderColorAttatchment = nullptr;
        // MTL::RenderCommandEncoder* m_FrameRenderCommandEncoder = nullptr;
        

        friend class MetalRendererAPI;
        friend class MacWindow;
        friend class MetalTexture2D;
        friend class MetalShader;
        friend class MetalVertexBuffer;
        friend class MetalIndexBuffer;
        friend class MetalImGuiRenderer;
        friend class MacImGuiLayer;
        friend class MetalVertexArray;
        friend class MetalFramebuffer;
    };
}
