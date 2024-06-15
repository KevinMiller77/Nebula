#pragma once

#include <Core/Window.h>
#include <Graphics/GraphicsContext.h>
#include <Platform/Graphics/Metal/MetalRendererAPI.h>

#include <Metal/Metal.hpp>
#include <Platform/OS/MacOS/MacImGuiLayer.h>

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
        
        bool m_AlphaBlendEnabled = false;
        bool m_DepthTestEnabled = false;
        bool m_MSAAEnabled = false;

        NS::Window* m_Window = nullptr;
        MTK::View* m_MainView = nullptr;
        MTL::RenderPassDescriptor* m_MainRenderPassDescriptor = nullptr;

        // Created once
        MTL::Device* m_Device = nullptr;
        MTL::CommandQueue* m_CommandQueue = nullptr;
        MTL::CommandBuffer* m_CommandBuffer = nullptr;

        // Bound every frame
        MTL::RenderPipelineState* m_RenderPipelineState = nullptr;
        MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;

        // Created every frame
        MTL::RenderCommandEncoder* m_RenderCommandEncoder = nullptr;
        
        // Window holds this data
        MTL::ClearColor m_ClearColor;
        MTL::Viewport m_Viewport;

        // Vertex Array binding
        MTL::Buffer* m_VertexBuffer = nullptr;
        MTL::Buffer* m_IndexBuffer = nullptr;
        
        friend class MetalRendererAPI;
        friend class MetalImGuiRenderer;
        friend class MacImGuiLayer;
    };
}
