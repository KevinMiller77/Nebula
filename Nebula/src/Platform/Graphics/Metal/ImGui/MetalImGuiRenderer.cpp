#include "MetalImGuiRenderer.h"

#include <Graphics/RendererConfig.h>
#include <Metal/MTLCommandBuffer.hpp>
#include <MetalKit/MTKView.hpp>
#include <Platform/Graphics/Metal/MetalRendererAPI.h>
#include <Platform/Graphics/Metal/MetalContext.h>
#include <imgui.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>

extern bool ImGui_ImplMetal_Init_CPP(MTL::Device* device);
extern void ImGui_ImplMetal_Shutdown_CPP();
extern void ImGui_ImplMetal_NewFrame_CPP(MTL::RenderPassDescriptor* renderPassDescriptor);
extern void ImGui_ImplMetal_RenderDrawData_CPP(ImDrawData* drawData, MTL::CommandBuffer* commandBuffer, MTL::RenderCommandEncoder* commandEncoder);

// MTL::CommandQueue* imgui_commandQueue = nullptr;
// MTL::CommandBuffer* imgui_commandBuffer = nullptr;
// MTL::RenderPassDescriptor* imgui_renderPassDescriptor = nullptr;
// MTL::RenderCommandEncoder* imgui_renderCommandEncoder = nullptr;

MTK::View* imgui_view = nullptr;

namespace Nebula{

    bool MetalImGuiRenderer::Init() { 
        Ref<MetalContext> s_Context = MetalRendererAPI::s_Context;
        imgui_view = MTK::View::alloc()->init(
            s_Context->m_Window->frame(),
            s_Context->m_Device
        );
        s_Context->m_Window->setContentView(s_Context->m_DefaultContentView);

        return ImGui_ImplMetal_Init_CPP(
            MetalRendererAPI::s_Context->m_Device
        );
    }

    void MetalImGuiRenderer::Shutdown() { 
        ImGui_ImplMetal_Shutdown_CPP();
    }

    void MetalImGuiRenderer::NewFrame() { 
        Ref<MetalContext> s_Context = MetalRendererAPI::s_Context;

        if (s_Context->m_BoundView != s_Context->m_DefaultContentView) {
            s_Context->m_BoundView = s_Context->m_DefaultContentView;
        }
        RendererConfig::Prepare();
        ImGui_ImplMetal_NewFrame_CPP(
            s_Context->m_RenderPassDescriptor
        );
    }

    void MetalImGuiRenderer::Render(ImDrawData* draw_data) {
        Ref<MetalContext> s_Context = MetalRendererAPI::s_Context;

        ImGui_ImplMetal_RenderDrawData_CPP(
            draw_data,
            s_Context->m_CommandBuffer,
            s_Context->m_RenderCommandEncoder
        );
        
        RendererConfig::Commit();
    }

}
