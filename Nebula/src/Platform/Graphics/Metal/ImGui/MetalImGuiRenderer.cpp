#include "MetalImGuiRenderer.h"

#include <Graphics/RendererConfig.h>
#include <Platform/Graphics/Metal/MetalRendererAPI.h>
#include <Platform/Graphics/Metal/MetalContext.h>
#include <imgui.h>

#include <Foundation/Foundation.hpp>
#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>

extern bool ImGui_ImplMetal_Init_CPP(MTL::Device* device);
extern void ImGui_ImplMetal_Shutdown_CPP();
extern void ImGui_ImplMetal_NewFrame_CPP(MTL::RenderPassDescriptor* renderPassDescriptor);
extern void ImGui_ImplMetal_RenderDrawData_CPP(ImDrawData* drawData, MTL::CommandBuffer* commandBuffer, MTL::RenderCommandEncoder* commandEncoder);

namespace Nebula{

    bool MetalImGuiRenderer::Init() { 
        return ImGui_ImplMetal_Init_CPP(
            MetalRendererAPI::s_Context->m_Device
        );
    }

    void MetalImGuiRenderer::Shutdown() { 
        ImGui_ImplMetal_Shutdown_CPP();
    }

    void MetalImGuiRenderer::NewFrame() { 
        RendererConfig::Prepare();
        RendererConfig::Clear();
        
        Ref<MetalContext> s_Context = MetalRendererAPI::s_Context;
        ImGui_ImplMetal_NewFrame_CPP(
            s_Context->m_MainRenderPassDescriptor
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
